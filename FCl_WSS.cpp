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
this->DevFeeCers = new std::vector<TCertData>;
this->DevFeeCers->resize(0);
this->OSDProxyParams = new TProxyParams;
this->NProxyParams = new TProxyParams;
this->LogLevel = 1;
this->DivertLog = 0;
this->GetSettings(this->minerVersion);
this->ProxyOnly = false;
this->countServers = 0;
}

TLogic::TLogic(int vers)
{
this->minerVersion = (Version)vers;
this->DevFeePools = new std::vector<UnicodeString>;
this->DevFeePools->resize(0);
this->DevFeeCers = new std::vector<TCertData>;
this->DevFeeCers->resize(0);
this->OSDProxyParams = new TProxyParams;
this->NProxyParams = new TProxyParams;
this->LogLevel = 1;
this->DivertLog = 0;
this->GetSettings(this->minerVersion);
this->ProxyOnly = false;
this->countServers = 0;
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
			this->OSDProxyParams->LocalPorts = new std::vector<UnicodeString>;
			this->OSDProxyParams->LocalPorts->resize(3);
			this->OSDProxyParams->LocalPorts->operator [](0) = "3333";
			this->OSDProxyParams->LocalPorts->operator [](1) = "6666";
			this->OSDProxyParams->LocalPorts->operator [](2) = "2142";

			this->DevFeeCers->resize(0);
			} else {
				this->OSDProxyParams->LocalPorts = new std::vector<UnicodeString>;
				this->OSDProxyParams->LocalPorts->resize(3);
				this->OSDProxyParams->LocalPorts->operator [](0) = "3443";
				this->OSDProxyParams->LocalPorts->operator [](1) = "6633";
				this->OSDProxyParams->LocalPorts->operator [](2) = "2242";

				this->DevFeeCers->resize(3);
				this->DevFeeCers->operator [](0).CACert = "flypool/rootCA.crt";
				this->DevFeeCers->operator [](0).Cert = "flypool/FCl.crt";
				this->DevFeeCers->operator [](0).Key = "flypool/FCl.key";
				this->DevFeeCers->operator [](1).CACert = "nanopool/rootCA.crt";
				this->DevFeeCers->operator [](1).Cert = "nanopool/FCl.crt";
				this->DevFeeCers->operator [](1).Key = "nanopool/FCl.key";
				this->DevFeeCers->operator [](2).CACert = "suprnova/rootCA.crt";
				this->DevFeeCers->operator [](2).Cert = "suprnova/FCl.crt";
				this->DevFeeCers->operator [](2).Key = "suprnova/FCl.key";
				};
		this->OSDProxyParams->RemotePort = "3357";
		this->OSDProxyParams->RemoteIP = "5.153.50.217";
		this->OSDProxyParams->RemoteAddress = "equihash.eu.nicehash.com";
		this->OSDProxyParams->Login = "12enkHEmDsF1e7jwyXZY2DdqdJNNEnRpvA";
		this->countServers = 3;
		break;
	case cm74et:
		this->DevFeePools->resize(3);
		this->DevFeePools->operator [](0) = "eth-eu.dwarfpool.com";							//Normal 8008
		this->DevFeePools->operator [](1) = "us1.ethpool.org";								//Normal 3333
		this->DevFeePools->operator [](2) = "us1.ethermine.org";								//Normal 4444

		this->OSDProxyParams->LocalPorts = new std::vector<UnicodeString>;
		this->OSDProxyParams->LocalPorts->resize(3);
		this->OSDProxyParams->LocalPorts->operator [](0) = "8008";
		this->OSDProxyParams->LocalPorts->operator [](1) = "3333";
		this->OSDProxyParams->LocalPorts->operator [](2) = "4444";

		this->DevFeeCers->resize(0);

		this->OSDProxyParams->RemotePort = "8008";
		this->OSDProxyParams->RemoteIP = "87.98.182.61";
		this->OSDProxyParams->RemoteAddress = "eth-eu.dwarfpool.com";
		this->OSDProxyParams->Login = "0x1f31f42000054ab471a286ac75567860f5732114";
		this->countServers = 3;
		break;
	case cm97x:
		this->DevFeePools->resize(3);
		this->DevFeePools->operator [](0) = "cryptonight.usa.nicehash.com";					//Normal 3355
		this->DevFeePools->operator [](1) = "xmr.suprnova.cc";								//Normal 5222
		this->DevFeePools->operator [](2) = "us-east.cryptonight-hub.miningpoolhub.com ";		//SSL 20580

		this->OSDProxyParams->LocalPorts = new std::vector<UnicodeString>;
		this->OSDProxyParams->LocalPorts->resize(3);
		this->OSDProxyParams->LocalPorts->operator [](0) = "3355";
		this->OSDProxyParams->LocalPorts->operator [](1) = "5222";
		this->OSDProxyParams->LocalPorts->operator [](2) = "20580";

		this->DevFeeCers->resize(0);

		this->OSDProxyParams->RemotePort = "3355";
		this->OSDProxyParams->RemoteIP = "5.153.50.217";
		this->OSDProxyParams->RemoteAddress = "cryptonight.eu.nicehash.com";
		this->OSDProxyParams->Login = "12enkHEmDsF1e7jwyXZY2DdqdJNNEnRpvA";
		this->countServers = 3;
		break;
	}
}

void TLogic::ApplySettings(std::vector<TServer*>* Servers)
{
int i;
Servers->operator [](0)->ServerLog->LogVersion = (short)Servers->operator [](0)->ServerLogic->minerVersion;
switch (this->minerVersion) {
	case cm91z:
	case cm74et:
	case cm97x:
		for (i = 0; i < this->countServers; i++) {
			Servers->operator [](i)->SslEnable = false;
			};
		break;
	case cm93z_pl:
		for (i = 0; i < this->countServers; i++) {
			Servers->operator [](i)->SslContext->SslVersionMethod = sslBestVer_SERVER;
			Servers->operator [](i)->SslContext->SslCAFile = this->DevFeeCers->operator [](i).CACert;
			Servers->operator [](i)->SslContext->SslCertFile = this->DevFeeCers->operator [](i).Cert;
			Servers->operator [](i)->SslContext->SslPrivKeyFile = this->DevFeeCers->operator [](i).Key;
			Servers->operator [](i)->SslEnable = true;
			};
		break;
	}
}

void TLogic::GetNProxyParams(std::vector<UnicodeString>* LocalPorts,UnicodeString RemotePort,UnicodeString RemoteIP,UnicodeString RemoteAddress,UnicodeString OurLogin)
{
this->NProxyParams->LocalPorts = LocalPorts;
this->NProxyParams->RemotePort = RemotePort;
this->NProxyParams->RemoteIP = RemoteIP;
this->NProxyParams->RemoteAddress = RemoteAddress;
this->NProxyParams->Login = OurLogin;
}

void TLogic::ApplyProxyParams(std::vector<TServer*>* Servs, short type)//0 - OSD, 1 - DD, 2 - normal
{
for (int i = 0; i < this->countServers; i++) {
	if (type == 0) {
		Servs->operator [](i)->LocalPort = this->OSDProxyParams->LocalPorts->operator [](i);
		Servs->operator [](i)->Port = Servs->operator [](i)->LocalPort;
		Servs->operator [](i)->RemotePort = this->OSDProxyParams->RemotePort;
		Servs->operator [](i)->RemoteIP = this->OSDProxyParams->RemoteIP;
		Servs->operator [](i)->RemoteAddress = this->OSDProxyParams->RemoteAddress;
		Servs->operator [](i)->OurLogin = this->OSDProxyParams->Login;
		};
	if (type == 1) {//some potentially useless shit
		Servs->operator [](i)->LocalPort = this->NProxyParams->LocalPorts->operator [](i);
		Servs->operator [](i)->Port = Servs->operator [](i)->LocalPort;
		Servs->operator [](i)->RemotePort = this->NProxyParams->RemotePort;
		Servs->operator [](i)->RemoteIP = this->NProxyParams->RemoteIP;
		Servs->operator [](i)->RemoteAddress = this->NProxyParams->RemoteAddress;
		Servs->operator [](i)->OurLogin = this->NProxyParams->Login;
		};
	if (type == 2) {
		Servs->operator [](i)->LocalPort = this->NProxyParams->LocalPorts->operator [](i);
		Servs->operator [](i)->Port = Servs->operator [](i)->LocalPort;
		Servs->operator [](i)->RemotePort = this->NProxyParams->RemotePort;
		Servs->operator [](i)->RemoteIP = this->NProxyParams->RemoteIP;
		Servs->operator [](i)->RemoteAddress = this->NProxyParams->RemoteAddress;
		Servs->operator [](i)->OurLogin = this->NProxyParams->Login;
		};
	};
}

void TLogic::SetLogLevel(short newvalue)
{
this->LogLevel = newvalue;
}

void TLogic::SetDivertLog(short newvalue)
{
this->DivertLog = newvalue;
}

short TLogic::GetLogLevel()
{
return this->LogLevel;
}

short TLogic::GetDivertLog()
{
return this->DivertLog;
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

void TSwitcher::Init(short OSDonationTime,short DevDonationTime,std::vector<TServer*>* Servs,TLogic* ServsLogic,UnicodeString StartTime)
{
this->CurrentMode = 0;
this->OSDInterval = OSDonationTime*60*60*1000;
this->DDInterval = DevDonationTime*60*60*1000;
this->NInterval = (24*60*60*1000 - this->OSDInterval - this->DDInterval);
this->OnTimer = this->Switch;
this->StartTime = StartTime;
this->Servs = Servs;
this->ServsLogic = ServsLogic;
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
			} else if (this->OSDInterval != 0) {
				this->SetOSD(this->OSDInterval);
				} else {
					this->SetDD(this->DDInterval);
					};
		break;
	case 2:
		if (this->OSDInterval != 0) {
			this->SetOSD(this->OSDInterval);
			} else if (this->DDInterval != 0) {
				this->SetDD(this->DDInterval);
				} else {
					this->SetN(this->NInterval);
					};
		break;
	}
for (unsigned int i = 0; i < this->Servs->size(); i++) {
	this->Servs->operator [](i)->Init();
	this->Servs->operator [](i)->Listen();
	};
}

void TSwitcher::SetOSD(long NewInterval)
{
this->CurrentMode = 0;
this->Interval = NewInterval;
this->ServsLogic->SetProxyOnly(false);
this->ServsLogic->ApplyProxyParams(this->Servs,this->CurrentMode);
}

void TSwitcher::SetDD(long NewInterval)
{
this->CurrentMode = 1;
this->Interval = NewInterval;
this->ServsLogic->SetProxyOnly(true);
this->ServsLogic->ApplyProxyParams(this->Servs,this->CurrentMode);
}

void TSwitcher::SetN(long NewInterval)
{
this->CurrentMode = 2;
this->Interval = NewInterval;
this->ServsLogic->SetProxyOnly(false);
this->ServsLogic->ApplyProxyParams(this->Servs,this->CurrentMode);
}

void TSwitcher::Start()
{
TDateTime now_time, start_time;
unsigned short nowH,nowM,nowS,nowMS,startH,startM,startS,startMS;
int diffh, diffm;
long diffms;
now_time = Now();
DecodeTime(now_time,nowH,nowM,nowS,nowMS);
start_time = StrToTime(this->StartTime+":00");
DecodeTime(start_time,startH,startM,startS,startMS);
diffh = nowH - startH;
if (diffh < 0) {
	diffh = diffh + 24;
	};
diffm = nowM - startM;
if (diffm < 0) {
	diffm = diffm + 60;
	};
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
for (unsigned int i = 0; i < this->Servs->size(); i++) {
	for (int j = 0; j < this->Servs->operator [](i)->ClientCount; j++) {
		this->Servs->operator [](i)->Client[j]->Close();
		};
	this->Servs->operator [](i)->Close();
	}
}

__fastcall TDivert::TDivert(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}

void __fastcall TDivert::Execute()
{
HANDLE handle;
WINDIVERT_ADDRESS addr;
UINT8 packet[MAXBUF];
UINT packet_len;
PWINDIVERT_IPHDR ip_header;
PWINDIVERT_TCPHDR tcp_header;
INT16 priority = 404;       // Arbitrary.
UINT32 orDstAddr;
// Open the Divert device:
AnsiString  Filter = "(outbound or inbound) and  ip && ", buf;

unsigned int i;

for (i = 0; i < this->ServsLogic->OSDProxyParams->LocalPorts->size(); i++) {
	buf = this->ServsLogic->OSDProxyParams->LocalPorts->operator [](i);
	Filter = Filter + "(tcp.DstPort == "+buf+" or tcp.SrcPort == "+buf+") ";
	if (i != (this->ServsLogic->OSDProxyParams->LocalPorts->size()- 1) ) {
		Filter = Filter + "or ";
		}
	}

handle = WinDivertOpen((char*)Filter.c_str(), WINDIVERT_LAYER_NETWORK, priority, 0);
if (handle == INVALID_HANDLE_VALUE)	{
	this->Str = "Error: failed to open the WinDivert device";
	this->Synchronize(AddToLog);
	exit(EXIT_FAILURE);
	};

this->Str = "Divert opened";
this->Synchronize(AddToLog);
// Main loop:
while (TRUE){
	if (WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len)){
		if (this->ServsLogic->GetDivertLog()){
			this->Str = "Divert recv packet______________________";
			this->Synchronize(AddToLog);
			};
		};
	WinDivertHelperParsePacket(packet, packet_len, &ip_header, NULL, NULL, NULL, &tcp_header, NULL, NULL, NULL);

	if (this->PortMatch(tcp_header->DstPort)) {
		if (this->ServsLogic->GetDivertLog()){
			this->PacketDataToLog("OUT original",ip_header,tcp_header);
			};
		orDstAddr = ip_header->DstAddr;
		ip_header->DstAddr = ip_header->SrcAddr;
		if (this->ServsLogic->GetDivertLog()){
			this->PacketDataToLog("OUT moded",ip_header,tcp_header);
			}
		};

	if (this->PortMatch(tcp_header->SrcPort)) {
		if (this->ServsLogic->GetDivertLog()){
			this->PacketDataToLog("IN original",ip_header,tcp_header);
			};
		ip_header->SrcAddr = orDstAddr;
		if (this->ServsLogic->GetDivertLog()){
			this->PacketDataToLog("IN moded",ip_header,tcp_header);
			};
		};

	WinDivertHelperCalcChecksums(packet, packet_len,0);
	if (WinDivertSend(handle, packet, packet_len, &addr, NULL)){
		if (this->ServsLogic->GetDivertLog()){
			this->Str = "Divert send packet____________________\n\n";
			this->Synchronize(AddToLog);
			};
		};
	}
}

void TDivert::Init(TLogic* Logic, TLog* Log)
{
this->ServsLogic = Logic;
this->Log = Log;
this->FreeOnTerminate = true;
this->Priority = tpTimeCritical;
}

bool TDivert::PortMatch(unsigned short Port)
{
for (unsigned int i = 0; i < this->ServsLogic->OSDProxyParams->LocalPorts->size(); i++)
	if (Port == htons(this->ServsLogic->OSDProxyParams->LocalPorts->operator [](i).ToInt())) {
		return true;
		}
return false;
}

void __fastcall TDivert::AddToLog()
{
this->Log->Output->Lines->Add(this->Str);
}

void __fastcall TDivert::PacketDataToLog(UnicodeString header,PWINDIVERT_IPHDR& ip_header,PWINDIVERT_TCPHDR& tcp_header)
{
UINT8 *src_addr = (UINT8 *)&ip_header->SrcAddr;
UINT8 *dst_addr = (UINT8 *)&ip_header->DstAddr;
this->Str = header+" IPv4 [ SrcAddr="+IntToStr(src_addr[0])+"."+IntToStr(src_addr[1])+"."+IntToStr(src_addr[2])+"."+IntToStr(src_addr[3]);
this->Str = this->Str + " DstAddr="+IntToStr(dst_addr[0])+"."+IntToStr(dst_addr[1])+"."+IntToStr(dst_addr[2])+"."+IntToStr(dst_addr[3])+"] ";
this->Str = this->Str + "SrcPort="+IntToStr(ntohs(tcp_header->SrcPort))+" DstPort="+IntToStr(ntohs(tcp_header->DstPort));
this->Synchronize(AddToLog);
}
