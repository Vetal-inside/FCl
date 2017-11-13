#ifndef FCl_WSSH
#define FCl_WSSH

#include "OverbyteIcsWndControl.hpp"
#include "OverbyteIcsWSocket.hpp"
#include "OverbyteIcsWSocketS.hpp"
#include "Unit2.h"
#include <vector>
#include <DBXJSON.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "windivert.h"
#define MAXBUF 0xFFFF
#define WINDIVERT_HELPER_NO_REPLACE 2048
#pragma comment(lib, "WinDivert_omf.lib")

class TLogic;

class TProxyParams
{
public:
std::vector<UnicodeString>* LocalPorts;
UnicodeString RemotePort;
UnicodeString RemoteIP;
UnicodeString RemoteAddress;
UnicodeString Login;
};

class TCertData
{
public:
UnicodeString CACert;
UnicodeString Cert;
UnicodeString Key;
};

class TClient: public TSslWSocketClient
{
public:
String Rcvd;
TWSocket* RemoteSocket;
inline __fastcall ~TClient(void);
};

class TServer : public TSslWSocketServer
{
typedef TWSocketServer inherited;
public:
UnicodeString LocalPort;
UnicodeString RemotePort;
UnicodeString RemoteIP;
UnicodeString RemoteAddress;
UnicodeString OurLogin;
TLog* ServerLog;
TLogic* ServerLogic;
inline __fastcall virtual TServer(System::Classes::TComponent* AOwner) : TSslWSocketServer(AOwner) { }
void inline __fastcall Init();
void __fastcall WSocketServerClientConnect(TObject *Sender, TWSocketClient *Client, WORD Error);
void __fastcall RemoteSessionConnected(TObject *Sender, WORD Error);
void __fastcall RemoteDataAvailable(TObject *Sender, WORD Error);
void __fastcall ClientDataAvailable(TObject *Sender, WORD Error);
void __fastcall RemoteSessionClosed(TObject *Sender, WORD Error);
void __fastcall BgException(TObject *Sender, Exception *E, bool &CanClose);
UnicodeString __fastcall ExchangeString(UnicodeString In);
void __fastcall SetLogLevel(short);
inline __fastcall virtual ~TServer(void) { }
};

enum Version {cm124z_pl = 0, cm74et, cm97x};
class TLogic
{
private:
Version minerVersion;
std::vector<UnicodeString>* DevFeePools;
std::vector<TCertData>* DevFeeCers;
short LogLevel;//0 - not, 1 - short, 2 - full
short DivertLog;
short ProxyOnly;

public:
TProxyParams* OSDProxyParams;
TProxyParams* NProxyParams;
short countServers;

TLogic();
TLogic(int);
void GetSettings(int);
void ApplySettings(std::vector<TServer*>*);
void GetNProxyParams(std::vector<UnicodeString>*, UnicodeString, UnicodeString, UnicodeString, UnicodeString);//          MUST
void ApplyProxyParams(std::vector<TServer*>*, short);//0 - OSD, 1 - DD, 2 - normal					  		//          BE
void SetLogLevel(short);																		//			SETTED
void SetDivertLog(short);																		//          ______
void SetProxyOnly(short);																		//          MANUALLY
short GetLogLevel();
short GetDivertLog();
short GetProxyOnly();
Version GetMinerVersion();
UnicodeString GetFeePoolDomainName(int);
int GetPoolsCount();
~TLogic();
};

class TSwitcher : public TTimer
{
private:
long OSDInterval;
long DDInterval;
long NInterval;
std::vector<TServer*>* Servs;
TLogic* ServsLogic;
short CurrentMode;//0 - OSD, 1 - DD, 2 - normal
UnicodeString StartTime;

void SetOSD(long);
void SetDD(long);
void SetN(long);

public:
inline __fastcall virtual TSwitcher(System::Classes::TComponent* AOwner) : TTimer(AOwner) { }
void __fastcall Switch(TObject *Sender);
void Init(short,short,std::vector<TServer*>*,TLogic*,UnicodeString);
void Start();
void Stop();
};

class TDivert : public TThread
{
protected:
void __fastcall Execute();

public:
TLogic* ServsLogic;
TLog* Log;
UnicodeString Str;
bool Needed;

__fastcall TDivert(bool CreateSuspended);
void Init(TLogic*, TLog*);
bool PortMatch(unsigned short);
void __fastcall AddToLog();
void __fastcall PacketDataToLog(UnicodeString,PWINDIVERT_IPHDR&,PWINDIVERT_TCPHDR&);
};
#endif
