//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Unit2.cpp"
#include "FCl_WSS.cpp"
#include "Settings.cpp"

//---------------------------------------------------------------------------

#pragma link "OverbyteIcsWndControl"
#pragma link "OverbyteIcsWSocket"
#pragma link "OverbyteIcsWSocketS"
#pragma resource "*.dfm"

TForm1 *Form1;
TForm2 *Form2;
TForm3 *Form3;
TLog* Log;
TServer* Server;
TSslContext* SslContext;
TLogic* Logic;
std::vector<TNetworkConfig>* NetworkConfigs;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenBtnClick(TObject *Sender)
{
if (ListenBtn->Tag == 0) {
	Server->Init(this->LocalPort->Text,this->RemotePort->Text,this->RealIP->Text,this->RemoteAddr->Text,this->edWorker->Text);
	Logic->logging = Form1->CheckBox1->Checked;
	Logic->SetServerLogic(Server);
	Server->Listen();
	LocalPort->Enabled = false;
	RemotePort->Enabled = false;
	RemoteAddr->Enabled = false;
	RealIP->Enabled = false;
	edWorker->Enabled = false;
	ComboBox1->Enabled = false;
	CheckBox1->Enabled = false;
	Button1->Enabled = false;
	Form3->Hide();
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
		Form1->CheckBox1->Enabled = true;
		Button1->Enabled = true;
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

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
Logic->UpdateSettings(Form1->ComboBox1->ItemIndex);
Form3->LoadNetworkSettings();
Form3->FillAdaptersData();
Form3->FillHostsData();
Form3->FillNetworkData();
switch (Form1->ComboBox1->ItemIndex) {
	case 0 :
		Form1->LocalPort->Text = "3333";
		break;
	case 1 :
		Form1->LocalPort->Text = "3443";
		break;
	;
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
Logic = new TLogic(Form1->ComboBox1->ItemIndex);
Logic->UpdateSettings(Form1->ComboBox1->ItemIndex);
NetworkConfigs = new std::vector<TNetworkConfig>(0);
SslContext = new TSslContext((TComponent*)Form1);
Server = new TServer((TComponent*)Form1);
Server->SslContext = SslContext;
Server->ServerLog = Log;
Server->ServerLogic = Logic;
Logic->SetServerLogic(Server);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
Form3->Show();
}
//---------------------------------------------------------------------------
