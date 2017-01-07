//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <vector>

// Need to link with Iphlpapi.lib and Ws2_32.lib
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
/* Note: could also use malloc() and free() */

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TComboBox *ComboBox3;
	TLabel *Label3;
	TLabel *Label4;
	TComboBox *ComboBox4;
	TMemo *Memo1;
	TMemo *Memo2;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ComboBox4Change(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
	void __fastcall LoadNetworkSettings();
	void __fastcall FillData();
};
//---------------------------------------------------------------------------
class TNetworkConfig
{
public:
UnicodeString AdapterName;
UnicodeString AdapterDescription;
UnicodeString FriendlyName;
UnicodeString ipAddress;
UnicodeString Mask;
UnicodeString Gateway;
std::vector<UnicodeString> DNS;
TNetworkConfig();
~TNetworkConfig();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
extern std::vector<TNetworkConfig>* NetworkConfigs;
//---------------------------------------------------------------------------
#endif
