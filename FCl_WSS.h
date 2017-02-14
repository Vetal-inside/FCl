#ifndef FCl_WSSH
#define FCl_WSSH

#include "OverbyteIcsWndControl.hpp"
#include "OverbyteIcsWSocket.hpp"
#include "OverbyteIcsWSocketS.hpp"
#include "Unit2.h"
#include <vector>
#include <DBXJSON.hpp>
#include <Vcl.ExtCtrls.hpp>

class TLogic;

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
void inline __fastcall Init(UnicodeString LocalPort,UnicodeString RemotePort,UnicodeString RemoteIP,UnicodeString RemoteAddress,UnicodeString OurLogin);
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

enum Version {cm91z = 0, cm93z_pl, cm74et, cm97x};
class TLogic
{
public:
Version minerVersion;
std::vector<UnicodeString>* Pools;
short LogLevel;//NAU//0 - not, 1 - short, 2 - full
bool ProxyOnly;//NAU

TLogic();
TLogic(int);
void UpdateSettings(int);
void SetServerLogic(TServer*);

~TLogic();
};

class TSwitcher : public TTimer
{
public:
long OSDInterval;
long DDInterval;
long NInterval;
UnicodeString RemotePort;
UnicodeString RemoteIP;
UnicodeString RemoteAddress;
UnicodeString OurLogin;
TServer* Serv;
short CurrentMode;//0 - OSD, 1 - DD, 2 - normal
UnicodeString StartTime;

inline __fastcall virtual TSwitcher(System::Classes::TComponent* AOwner) : TTimer(AOwner) { }
void Init(short,short,TServer*,UnicodeString);
void SetOSD(long);
void SetDD(long);
void SetN(long);
void __fastcall Switch(TObject *Sender);
void Start();
void Stop();
};
#endif
