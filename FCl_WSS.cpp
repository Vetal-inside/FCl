#include "FCl_WSS.h"

inline __fastcall TClient::~TClient(void)
{
if (!Application->Terminated) {
	((TServer*)this->Owner)->ServerLog->Header->operator =("Connection Closed");
	((TServer*)this->Owner)->ServerLog->Add("");
	}
this->RemoteSocket->Free();
}

void inline __fastcall TServer::Init()
{
this->LineEnd = "#13#10";
this->Proto = "tcp";
this->LocalAddr = "0.0.0.0";
this->LocalAddr6 = "::";
this->SocksLevel = "5";
this->Banner = "";
this->BannerTooBusy = "";
this->ClientClass = __classid(TClient);
this->Addr = "0.0.0.0";
this->OnBgException = this->BgException;
this->OnClientConnect = this->WSocketServerClientConnect;
}

void __fastcall TServer::WSocketServerClientConnect(TObject *Sender, TWSocketClient *Client, WORD Error)
{
((TClient*)Client)->Rcvd = "";
Client->OnDataAvailable = this->ClientDataAvailable;
Client->OnBgException = this->BgException;
Client->LineMode = false;
Client->LineEnd = "\r\n";
((TClient*)Client)->RemoteSocket = new TWSocket(((TClient*)Client));
((TClient*)Client)->RemoteSocket->Port = this->RemotePort;
((TClient*)Client)->RemoteSocket->LineMode = false;
((TClient*)Client)->RemoteSocket->LineEnd = "\r\n";
((TClient*)Client)->RemoteSocket->OnSessionConnected = this->RemoteSessionConnected;
((TClient*)Client)->RemoteSocket->OnDataAvailable = this->RemoteDataAvailable;
((TClient*)Client)->RemoteSocket->OnSessionClosed = this->RemoteSessionClosed;
((TClient*)Client)->RemoteSocket->OnBgException = this->BgException;
((TClient*)Client)->RemoteSocket->Addr = this->RemoteIP;
((TClient*)Client)->RemoteSocket->Connect();
}

void __fastcall TServer::RemoteSessionConnected(TObject *Sender, WORD Error)
{
if (Error != 0) {exit(Error);};
if (this->ServerLogic->GetLogLevel()>0) {
	this->ServerLog->Header->operator =("Connection Opened");
	this->ServerLog->Add("");
	}
if (((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd != "") {
	if (this->ServerLogic->GetLogLevel()>0) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd = ExchangeString(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	if (this->ServerLogic->GetLogLevel()>0) {
		this->ServerLog->Header->operator =("Sent to pool");
		this->ServerLog->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->RemoteSocket->SendStr(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd="";
	}
}

void __fastcall TServer::RemoteDataAvailable(TObject *Sender, WORD Error)
{
UnicodeString FromRemote;
if (Error != 0) {exit(Error);};
FromRemote = ((TClient*)(((TWSocket*)Sender)))->ReceiveStr();
if (FromRemote != "") {
	if (this->ServerLogic->GetLogLevel()>0) {
		this->ServerLog->Header->operator =("From pool");
		this->ServerLog->Add(FromRemote);
		}
	if (((TClient*)(((TWSocket*)Sender)->Owner))->State == wsConnected) {
		((TClient*)(((TWSocket*)Sender)->Owner))->SendStr(FromRemote);
		} else {
			if (this->ServerLogic->GetLogLevel()>0) {
				this->ServerLog->Header->operator =("Error: Local has closed");
				this->ServerLog->Add("");
				}
			((TClient*)(((TWSocket*)Sender)->Owner))->CloseDelayed();
			};
	}
}

UnicodeString __fastcall TServer::ExchangeString(UnicodeString In)
{
UnicodeString Out="";
TJSONObject *json_root, *json_subroot;
TJSONArray *json_array;
int i,j=1;//split complex JSON string into simple
std::vector<UnicodeString> InArr;
InArr.resize(0);
for (i = 1; i < In.Length()+1; i++) {
	if (In[i]=='\n') {
		InArr.resize(InArr.size()+1);
		InArr.operator [](InArr.size()-1).operator =(In.SubString(j,i-j));
		j=i+1;
		}
	}
InArr.resize(InArr.size()+1);
InArr.operator [](InArr.size()-1).operator =(In.SubString(j,i-j));
__try {
	for (i = 0; i < (int)InArr.size(); i++) {//and work with each of them
		json_root = (TJSONObject*) TJSONObject::ParseJSONValue(TEncoding::ASCII->GetBytes(InArr.operator [](i)),0);
		if ((json_root)&&(json_root->Get("id"))) {
			switch (this->ServerLogic->GetMinerVersion()) {
				case cm91z:
				case cm93z_pl:
					if (json_root->Get("id")->JsonValue->ToString() == "1") {//mining.subscribe BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.subscribe\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(2)->ToString(), "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(3)->ToString(), "\""+this->RemotePort+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.subscribe END
					if ((json_root->Get("id")->JsonValue->ToString() == "2")&&(!this->ServerLogic->GetProxyOnly())) {//mining.authorize BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.authorize\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(1)->ToString(), "\"20000\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.authorize END
					if ((json_root->Get("id")->JsonValue->ToString() == "4")&&(!this->ServerLogic->GetProxyOnly())) {//mining.submit BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.submit\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.submit END
					break;
				case cm74et:
					if ((json_root->Get("id")->JsonValue->ToString() == "2")&&(!this->ServerLogic->GetProxyOnly())) {//eth_submitLogin BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_submitLogin\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(1)->ToString(), "\"x\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//eth_submitLogin END
					break;
				case cm97x:
					if ((json_root->Get("id")->JsonValue->ToString() == "1")&&(!this->ServerLogic->GetProxyOnly())) {//login BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"login\"")){
							json_subroot = (TJSONObject*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_subroot->Get("login")->JsonValue->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_subroot->Get("pass")->JsonValue->ToString(), "\"x\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//login END
					break;
				}
			Out.operator +=(InArr.operator [](i));
			Out.SetLength(Out.Length()+1);
			Out.operator [](Out.Length()) = '\n';
			}
		}
	}__finally {
		if (json_root) {json_root->Free();};
		};
return Out;
}

void __fastcall TServer::ClientDataAvailable(TObject *Sender, WORD Error)
{
if (Error != 0) {exit(Error);};
((TClient*)Sender)->Rcvd = ((TClient*)Sender)->Rcvd + ((TClient*)Sender)->ReceiveStr();
if ((((TClient*)Sender)->RemoteSocket->State == wsConnected) &&(((TClient*)Sender)->Rcvd != "")) {
	if (this->ServerLogic->GetLogLevel()>0) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->Rcvd = ExchangeString(((TClient*)Sender)->Rcvd);
	if (this->ServerLogic->GetLogLevel()>0) {
		this->ServerLog->Header->operator =("Sent to pool");
		this->ServerLog->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->RemoteSocket->SendStr(((TClient*)Sender)->Rcvd);
	((TClient*)Sender)->Rcvd = "";
	}
}

void __fastcall TServer::RemoteSessionClosed(TObject *Sender, WORD Error)
{
((TClient*)(((TWSocket*)Sender)->Owner))->Shutdown(1);
}

void __fastcall TServer::BgException(TObject *Sender, Exception *E, bool &CanClose)
{
CanClose = true;
}

void __fastcall TServer::SetLogLevel(short level)
{
this->ServerLogic->SetLogLevel(level);
this->ServerLog->LogLevel = level;
}

TLogic::TLogic()
{
this->minerVersion = cm91z;
this->DevFeePools = new std::vector<UnicodeString>;
this->DevFeePools->resize(0);
this->OSDProxyParams = new TProxyParams;
this->NProxyParams = new TProxyParams;
this->LogLevel = 1;
this->GetSettings(this->minerVersion);
this->ProxyOnly = false;
}

TLogic::TLogic(int vers)
{
this->minerVersion = (Version)vers;
this->DevFeePools = new std::vector<UnicodeString>;
this->DevFeePools->resize(0);
this->OSDProxyParams = new TProxyParams;
this->NProxyParams = new TProxyParams;
this->LogLevel = 1;
this->GetSettings(this->minerVersion);
this->ProxyOnly = false;
}

void TLogic::GetSettings(int vers)
{
this->minerVersion = (Version)vers;
switch (this->minerVersion) {
	case cm91z:
	case cm93z_pl:
		this->DevFeePools->resize(4);
		this->DevFeePools->operator [](0) = "us1-zcash.flypool.org";							//Normal 3333		SSL 3443
		this->DevFeePools->operator [](1) = "eu1-zcash.flypool.org";							//Normal 3333		SSL 3443
		this->DevFeePools->operator [](2) = "zec-eu1.nanopool.org";							//Normal 6666		SSL 6633
		this->DevFeePools->operator [](3) = "zec.suprnova.cc";								//Normal 2142		SSL 2242

		if (this->minerVersion == cm91z) {
			this->OSDProxyParams->LocalPort = "3333";
			} else {
				this->OSDProxyParams->LocalPort = "3443";
				};
		this->OSDProxyParams->RemotePort = "3357";
		this->OSDProxyParams->RemoteIP = "5.153.50.217";
		this->OSDProxyParams->RemoteAddress = "equihash.eu.nicehash.com";
		this->OSDProxyParams->Login = "12enkHEmDsF1e7jwyXZY2DdqdJNNEnRpvA";
		break;
	case cm74et:
		this->DevFeePools->resize(3);
		this->DevFeePools->operator [](0) = "eth-eu.dwarfpool.com";							//Normal 8008
		this->DevFeePools->operator [](1) = "us1.ethpool.org";								//Normal 3333
		this->DevFeePools->operator [](2) = "us1.ethermine.org";								//Normal 4444

		this->OSDProxyParams->LocalPort = "8008";
		this->OSDProxyParams->RemotePort = "8008";
		this->OSDProxyParams->RemoteIP = "87.98.182.61";
		this->OSDProxyParams->RemoteAddress = "eth-eu.dwarfpool.com";
		this->OSDProxyParams->Login = "0x1f31f42000054ab471a286ac75567860f5732114";
		break;
	case cm97x:
		this->DevFeePools->resize(3);
		this->DevFeePools->operator [](0) = "cryptonight.usa.nicehash.com";					//Normal 3355
		this->DevFeePools->operator [](1) = "xmr.suprnova.cc";								//Normal 5222
		this->DevFeePools->operator [](2) = "us-east.cryptonight-hub.miningpoolhub.com ";		//SSL 20580

		this->OSDProxyParams->LocalPort = "3355";
		this->OSDProxyParams->RemotePort = "3355";
		this->OSDProxyParams->RemoteIP = "5.153.50.217";
		this->OSDProxyParams->RemoteAddress = "cryptonight.eu.nicehash.com";
		this->OSDProxyParams->Login = "12enkHEmDsF1e7jwyXZY2DdqdJNNEnRpvA";
		break;
	}
}

void TLogic::ApplySettings(TServer* Server)
{
Server->ServerLog->LogVersion = (short)Server->ServerLogic->minerVersion;
switch (this->minerVersion) {
	case cm91z:
	case cm74et:
	case cm97x:
		Server->SslEnable = false;
		break;
	case cm93z_pl:
		Server->SslContext->SslVersionMethod = sslBestVer_SERVER;
		Server->SslContext->SslCAFile = "CA.pem";
		Server->SslContext->SslCertFile = "FCl.pem";
		Server->SslContext->SslPrivKeyFile = "FCl.key";
		Server->SslEnable = true;
		break;
	}
}

void TLogic::GetNProxyParams(UnicodeString LocalPort,UnicodeString RemotePort,UnicodeString RemoteIP,UnicodeString RemoteAddress,UnicodeString OurLogin)
{
this->NProxyParams->LocalPort = LocalPort;
this->NProxyParams->RemotePort = RemotePort;
this->NProxyParams->RemoteIP = RemoteIP;
this->NProxyParams->RemoteAddress = RemoteAddress;
this->NProxyParams->Login = OurLogin;
}

void TLogic::ApplyProxyParams(TServer* Serv, short type)//0 - OSD, 1 - DD, 2 - normal
{
if (type == 0) {
	Serv->LocalPort = this->OSDProxyParams->LocalPort;
	Serv->Port = Serv->LocalPort;
	Serv->RemotePort = this->OSDProxyParams->RemotePort;
	Serv->RemoteIP = this->OSDProxyParams->RemoteIP;
	Serv->RemoteAddress = this->OSDProxyParams->RemoteAddress;
	Serv->OurLogin = this->OSDProxyParams->Login;
	};
if (type == 2) {
	Serv->LocalPort = this->NProxyParams->LocalPort;
	Serv->Port = Serv->LocalPort;
	Serv->RemotePort = this->NProxyParams->RemotePort;
	Serv->RemoteIP = this->NProxyParams->RemoteIP;
	Serv->RemoteAddress = this->NProxyParams->RemoteAddress;
	Serv->OurLogin = this->NProxyParams->Login;
	};
}

void TLogic::SetLogLevel(short newvalue)
{
this->LogLevel = newvalue;
}

short TLogic::GetLogLevel()
{
return this->LogLevel;
}

void TLogic::SetProxyOnly(short newvalue)
{
this->ProxyOnly = newvalue;
}

short TLogic::GetProxyOnly()
{
return this->ProxyOnly;
}

Version TLogic::GetMinerVersion()
{
return this->minerVersion;
}

UnicodeString TLogic::GetFeePoolDomainName(int inx)
{
return this->DevFeePools->operator [](inx);
}

int TLogic::GetPoolsCount()
{
return (int)this->DevFeePools->size();
}

TLogic::~TLogic()
{
delete[] this->DevFeePools;
delete this->OSDProxyParams;
delete this->NProxyParams;
}

void TSwitcher::Init(short OSDonationTime,short DevDonationTime,TServer* Serv,UnicodeString StartTime)
{
this->CurrentMode = 0;
this->OSDInterval = OSDonationTime*60*60*1000;
this->DDInterval = DevDonationTime*60*60*1000;
this->NInterval = (24*60*60*1000 - this->OSDInterval - this->DDInterval);
this->OnTimer = this->Switch;
this->StartTime = StartTime;
this->Serv = Serv;
}

void __fastcall TSwitcher::Switch(TObject *Sender)
{
this->Stop();
switch (this->CurrentMode) {
	case 0:
		if (this->DDInterval != 0) {
			this->SetDD(this->DDInterval);
			} else if (this->NInterval != 0) {
				this->SetN(this->NInterval);
				} else {
					this->SetOSD(this->OSDInterval);
					};
		break;
	case 1:
		if (this->NInterval != 0) {
			this->SetN(this->NInterval);
			} else {
				this->SetOSD(this->OSDInterval);
				};
		break;
	case 2:
        this->SetOSD(this->OSDInterval);
		break;
	}
this->Serv->Listen();
}

void TSwitcher::SetOSD(long NewInterval)
{
this->CurrentMode = 0;
this->Interval = NewInterval;
this->Serv->ServerLogic->SetProxyOnly(false);
this->Serv->ServerLogic->ApplyProxyParams(this->Serv,this->CurrentMode);
}

void TSwitcher::SetDD(long NewInterval)
{
this->CurrentMode = 1;
this->Interval = NewInterval;
this->Serv->ServerLogic->SetProxyOnly(true);
}

void TSwitcher::SetN(long NewInterval)
{
this->CurrentMode = 2;
this->Interval = NewInterval;
this->Serv->ServerLogic->SetProxyOnly(false);
this->Serv->ServerLogic->ApplyProxyParams(this->Serv,this->CurrentMode);
}

void TSwitcher::Start()
{
TDateTime diff;
UnicodeString sdiff;
int diffh, diffm, diffs, i , j = 1;
long diffms;
diff = Now() - StrToTime(this->StartTime+":00");
sdiff = TimeToStr(diff);
if (sdiff.Length()==8) {
	diffh = StrToInt(sdiff[1])*10+StrToInt(sdiff[2]);
	diffm = StrToInt(sdiff[4])*10+StrToInt(sdiff[5]);
	} else
		{
		diffh = StrToInt(sdiff[1]);
		diffm = StrToInt(sdiff[3])*10+StrToInt(sdiff[4]);
		}
diffms = (diffh*60 + diffm)*60*1000;
if ((diffms - this->OSDInterval) > 0) {
	diffms = diffms - this->OSDInterval;
	if ((diffms - this->DDInterval) > 0) {
		diffms = diffms - this->DDInterval;
		this->SetN(this->NInterval - diffms);
		} else {
			this->SetDD(this->DDInterval - diffms);
			}
	} else {
		this->SetOSD(this->OSDInterval - diffms);
		}
}

void TSwitcher::Stop()
{
this->Interval = 0;
for (int i = 0; i < this->Serv->ClientCount; i++) {
	this->Serv->Client[i]->Close();
	};
this->Serv->Close();
}
