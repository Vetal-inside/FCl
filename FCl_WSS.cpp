#include "FCl_WSS.h"

inline __fastcall TClient::~TClient(void)
{
if (!Application->Terminated) {
	((TServer*)this->Owner)->ServerLog->Header->operator =("Connection Closed");
	((TServer*)this->Owner)->ServerLog->Add("");
	}
this->RemoteSocket->Free();
}

void inline __fastcall TServer::Init(UnicodeString LocalPort,UnicodeString RemotePort,UnicodeString RemoteIP,UnicodeString RemoteAddress,UnicodeString OurLogin)
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
this->LocalPort = LocalPort;
this->Port = this->LocalPort;
this->RemotePort = RemotePort;
this->RemoteIP = RemoteIP;
this->RemoteAddress = RemoteAddress;
this->OurLogin = OurLogin;
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
if (this->ServerLogic->LogLevel>0) {
	this->ServerLog->Header->operator =("Connection Opened");
	this->ServerLog->Add("");
	}
if (((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd != "") {
	if (this->ServerLogic->LogLevel>0) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd = ExchangeString(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	if (this->ServerLogic->LogLevel>0) {
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
	if (this->ServerLogic->LogLevel>0) {
		this->ServerLog->Header->operator =("From pool");
		this->ServerLog->Add(FromRemote);
		}
	if (((TClient*)(((TWSocket*)Sender)->Owner))->State == wsConnected) {
		((TClient*)(((TWSocket*)Sender)->Owner))->SendStr(FromRemote);
		} else {
			if (this->ServerLogic->LogLevel>0) {
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
			switch (this->ServerLogic->minerVersion) {
				case cm91z:
				case cm93z_pl:
					if (json_root->Get("id")->JsonValue->ToString() == "1") {//mining.subscribe BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.subscribe\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(2)->ToString(), "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(3)->ToString(), "\""+this->RemotePort+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.subscribe END
					if (json_root->Get("id")->JsonValue->ToString() == "2") {//mining.authorize BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.authorize\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(1)->ToString(), "\"x\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.authorize END
					if (json_root->Get("id")->JsonValue->ToString() == "4") {//mining.submit BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.submit\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//mining.submit END
					break;
				case cm74et:
					if (json_root->Get("id")->JsonValue->ToString() == "2") {//eth_submitLogin BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_submitLogin\"")){
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(0)->ToString(), "\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							InArr.operator [](i) = StringReplace(InArr.operator [](i),json_array->Get(1)->ToString(), "\"x\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
							}
						}//eth_submitLogin END
					break;
				case cm97x:
					if (json_root->Get("id")->JsonValue->ToString() == "1") {//login BEGIN
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
	if (this->ServerLogic->LogLevel>0) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->Rcvd = ExchangeString(((TClient*)Sender)->Rcvd);
	if (this->ServerLogic->LogLevel>0) {
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
this->ServerLogic->LogLevel = level;
this->ServerLog->LogLevel = level;
}

TLogic::TLogic()
{
this->minerVersion = cm91z;
this->Pools = new std::vector<UnicodeString>;
this->Pools->resize(0);
this->LogLevel = 1;
this->Methods = new std::vector<UnicodeString>;
this->Methods->resize(0);
this->UpdateSettings(this->minerVersion);
}

TLogic::TLogic(int vers)
{
this->minerVersion = (Version)vers;
this->Pools = new std::vector<UnicodeString>;
this->Pools->resize(0);
this->LogLevel = 1;
this->Methods = new std::vector<UnicodeString>;
this->Methods->resize(0);
this->UpdateSettings(this->minerVersion);
}

void TLogic::UpdateSettings(int vers)
{
this->minerVersion = (Version)vers;
switch (this->minerVersion) {
	case cm91z:
	case cm93z_pl:
		this->Pools->resize(4);
		this->Pools->operator [](0) = "us1-zcash.flypool.org";							//Normal 3333		SSL 3443
		this->Pools->operator [](1) = "eu1-zcash.flypool.org";							//Normal 3333		SSL 3443
		this->Pools->operator [](2) = "zec-eu1.nanopool.org";							//Normal 6666		SSL 6633
		this->Pools->operator [](3) = "zec.suprnova.cc";								//Normal 2142		SSL 2242
		this->Methods->resize(7);
		this->Methods->operator [](0) = "\"mining.subscribe\"";							//id=1
		this->Methods->operator [](1) = "\"mining.authorize\"";							//id=2
		this->Methods->operator [](2) = "didnt_seen";		  							//id=3
		this->Methods->operator [](3) = "\"mining.submit\"";							//id=4
		this->Methods->operator [](4) = "\"mining.extranonce.subscribe\"";				//id=5
		this->Methods->operator [](5) = "didnt_seen";		 							//id=6
		this->Methods->operator [](6) = "\"mining.set_target\"";						//id=null
		break;
	case cm74et:
		this->Pools->resize(3);
		this->Pools->operator [](0) = "eth-eu.dwarfpool.com";							//Normal 8008
		this->Pools->operator [](1) = "us1.ethpool.org";								//Normal 3333
		this->Pools->operator [](2) = "us1.ethermine.org";								//Normal 4444
		this->Methods->resize(7);
		this->Methods->operator [](0) = "didnt_seen";		  							//id=1
		this->Methods->operator [](1) = "\"eth_submitLogin\"";							//id=2
		this->Methods->operator [](2) = "\"eth_getWork\"";								//id=3
		this->Methods->operator [](3) = "\"eth_submitWork\"";							//id=4
		this->Methods->operator [](4) = "didnt_seen";		  							//id=5
		this->Methods->operator [](5) = "\"eth_submitHashrate\"";						//id=6
		this->Methods->operator [](6) = "didnt_seen";			  						//id=null
		break;
	case cm97x:
		this->Pools->resize(3);
		this->Pools->operator [](0) = "cryptonight.usa.nicehash.com";					//SSL 3355
		this->Pools->operator [](1) = "xmr.suprnova.cc";								//SSL 5222
		this->Pools->operator [](2) = "us-east.cryptonight-hub.miningpoolhub.com ";		//SSL 20580
		this->Methods->resize(7);
		this->Methods->operator [](0) = "didnt_seen";		  							//id=1
		this->Methods->operator [](1) = "\"login\"";									//id=2
		this->Methods->operator [](2) = "didnt_seen";									//id=3
		this->Methods->operator [](3) = "didnt_seen";									//id=4
		this->Methods->operator [](4) = "\"submit\""; 		  							//id=5
		this->Methods->operator [](5) = "didnt_seen";									//id=6
		this->Methods->operator [](6) = "didnt_seen";			  						//id=null
		break;
	}

}

void TLogic::SetServerLogic(TServer* Server)
{
Server->ServerLog->LogVersion = (short)Server->ServerLogic->minerVersion;
Server->ServerLog->Methods = Server->ServerLogic->Methods;
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

TLogic::~TLogic()
{
delete[] this->Pools;
}
