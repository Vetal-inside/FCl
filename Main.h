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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *RemotePort;
	TButton *ListenBtn;
	TEdit *LocalPort;
	TEdit *edWorker;
	TEdit *RemoteAddr;
	TEdit *RealIP;
	TLabel *Label4;
	TLabel *Label5;
	TComboBox *ComboBox1;
	TLabel *Label6;
	TButton *Button1;
	TComboBox *ComboBox2;
	TLabel *Label7;
	void __fastcall ListenBtnClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall LoadFromFile();

private:
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	bool FromFile;
	bool EnableLocalPort;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
