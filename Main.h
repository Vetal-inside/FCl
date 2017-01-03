//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "OverbyteIcsWndControl.hpp"
#include "OverbyteIcsWSocket.hpp"
#include "OverbyteIcsWSocketS.hpp"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
/*class TClient: public TWSocketClient
{
public:
String Rcvd;
TWSocket* RemoteSocket;
inline __fastcall ~TClient(void);
};*/
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *RemotePort;
	TButton *ListenBtn;
	TCheckBox *CheckBox1;
	TEdit *LocalPort;
	TEdit *edWorker;
	TEdit *RemoteAddr;
	TEdit *RealIP;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall ListenBtnClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
