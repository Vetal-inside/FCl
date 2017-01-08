#include "FCl_WSS.h"
#include "Unit2.h"

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
this->logging = logging;
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
if (this->logging) {
	this->ServerLog->Header->operator =("Connection Opened");
	this->ServerLog->Add("");
	}
if (((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd != "") {
   	if (this->logging) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd = ExchangeString(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	if (this->logging) {
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
	if (this->logging) {
		this->ServerLog->Header->operator =("From pool");
		this->ServerLog->Add(FromRemote);
		}
	if (((TClient*)(((TWSocket*)Sender)->Owner))->State == wsConnected) {
		((TClient*)(((TWSocket*)Sender)->Owner))->SendStr(FromRemote);
		} else {
			if (this->logging) {
				this->ServerLog->Header->operator =("Error: Local has closed");
				this->ServerLog->Add("");
				}
			((TClient*)(((TWSocket*)Sender)->Owner))->CloseDelayed();
			};
	}
}

UnicodeString __fastcall TServer::ExchangeString(UnicodeString si)
{
UnicodeString s;
s = si;
s = StringReplace(s, "\"zec-eu1.nanopool.org\"", "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"us1-zcash.flypool.org\"", "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"eu1-zcash.flypool.org\"", "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"zec.suprnova.cc\"", "\""+this->RemoteAddress+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));

s = StringReplace(s, "\""+this->LocalPort+"\"", "\""+this->RemotePort+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));

s = StringReplace(s,"\"t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr\"","\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t\"","\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv\"","\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP\"","\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"1LmMNkiEvjapn5PRY8A9wypcWJveRrRGWr\"","\""+this->OurLogin+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
return s;
}

void __fastcall TServer::ClientDataAvailable(TObject *Sender, WORD Error)
{
if (Error != 0) {exit(Error);};
((TClient*)Sender)->Rcvd = ((TClient*)Sender)->Rcvd + ((TClient*)Sender)->ReceiveStr();
if ((((TClient*)Sender)->RemoteSocket->State == wsConnected) &&(((TClient*)Sender)->Rcvd != "")) {
   	if (this->logging) {
		this->ServerLog->Header->operator =("Rcvd from miner");
		this->ServerLog->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->Rcvd = ExchangeString(((TClient*)Sender)->Rcvd);
	if (this->logging) {
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

TLogic::TLogic()
{
this->minerVersion = cm91;
this->Pools = new std::vector<UnicodeString>;
this->Pools->resize(0);
this->logging = true;
this->UpdateSettings(this->minerVersion);
}

TLogic::TLogic(int vers)
{
this->minerVersion = (Version)vers;
this->Pools = new std::vector<UnicodeString>;
this->Pools->resize(0);
this->logging = true;
this->UpdateSettings(this->minerVersion);
}

void TLogic::UpdateSettings(int vers)
{
this->minerVersion = (Version)vers;
switch (this->minerVersion) {
	case cm91:
	case cm93:
		this->Pools->resize(4);
		this->Pools->operator [](0) = "us1-zcash.flypool.org";//Normal 3333		SSL 3443
		this->Pools->operator [](1) = "eu1-zcash.flypool.org";//Normal 3333		SSL 3443
		this->Pools->operator [](2) = "zec-eu1.nanopool.org";//	Normal 6666     SSL 6633
		this->Pools->operator [](3) = "zec.suprnova.cc";//		Normal 2142		SSL 2242
		break;
	}

}

void TLogic::SetServerLogic(TServer* Server)
{
Server->logging = this->logging;
switch (this->minerVersion) {
	case cm91:
		Server->SslEnable = false;
		break;
	case cm93:
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
