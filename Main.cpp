//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Unit2.cpp"
#include "FCl_WSS.cpp"

//---------------------------------------------------------------------------

#pragma link "OverbyteIcsWndControl"
#pragma link "OverbyteIcsWSocket"
#pragma link "OverbyteIcsWSocketS"
#pragma resource "*.dfm"

TForm1 *Form1;
TForm2 *Form2;
TLog* Log;
TServer* Server;
TSslContext* SslContext;
TLogic* Logic;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenBtnClick(TObject *Sender)
{
if (ListenBtn->Tag == 0) {
	Server->Init(this->LocalPort->Text,this->RemotePort->Text,this->RealIP->Text,this->RemoteAddr->Text,this->edWorker->Text,this->CheckBox1->Checked);
	Server->Listen();
	Logic->SetServerLogic(Server);
	LocalPort->Enabled = false;
	RemotePort->Enabled = false;
	RemoteAddr->Enabled = false;
	RealIP->Enabled = false;
	edWorker->Enabled = false;
	ComboBox1->Enabled = false;
	ListenBtn->Caption = "Cancel";
	ListenBtn->Tag = 1;
	}else {
		for (int i = 0; i < Server->ClientCount; i++) {
			Server->Client[i]->Close();
			}
		Server->Close();
		Form1->LocalPort->Enabled = true;
		Form1->RemotePort->Enabled = true;
		Form1->RemoteAddr->Enabled = true;
		Form1->RealIP->Enabled = true;
		Form1->edWorker->Enabled = true;
		Form1->ComboBox1->Enabled = true;
		Form1->ListenBtn->Caption = "Listen";
		Form1->ListenBtn->Tag = 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
		  bool &Resize)
{
Resize = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
Logic = new TLogic(Form1->ComboBox1->ItemIndex);
SslContext = new TSslContext((TComponent*)Form1);
Server = new TServer((TComponent*)Form1);
Server->SslContext = SslContext;
Server->ServerLog = Log;
Server->ServerLogic = Logic;
Logic->SetServerLogic(Server);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
Logic->UpdateVersion(Form1->ComboBox1->ItemIndex);
}
//---------------------------------------------------------------------------
