//---------------------------------------------------------------------------
#include <vcl.h>
#include <fstream>
#include <stdio.h>
#include <io.h>
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
TSwitcher* Switcher;
std::vector<TNetworkConfig>* NetworkConfigs;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
this->EnableLocalPort = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenBtnClick(TObject *Sender)
{
if (ListenBtn->Tag == 0) {
	Server->Init();
	Server->SetLogLevel(Form1->ComboBox2->ItemIndex);
	Logic->GetSettings(Form1->ComboBox1->ItemIndex);
	Logic->ApplySettings(Server);
	Logic->GetNProxyParams(this->LocalPort->Text,this->RemotePort->Text,this->RealIP->Text,this->RemoteAddr->Text,this->edWorker->Text);
	Logic->ApplyProxyParams(Server,2);
	Switcher->Init(Form1->TrackBar1->Position + 1,Form1->TrackBar2->Position,Server,Form1->StartTime->Text);
	Switcher->Start();
	Server->Listen();
	LocalPort->Enabled = false;
	RemotePort->Enabled = false;
	RemoteAddr->Enabled = false;
	RealIP->Enabled = false;
	edWorker->Enabled = false;
	ComboBox1->Enabled = false;
	ComboBox2->Enabled = false;
	Button1->Enabled = false;
	StartTime->Enabled = false;
	TrackBar1->Enabled = false;
	TrackBar2->Enabled = false;
	Form3->Hide();
	ListenBtn->Caption = "Cancel";
	ListenBtn->Tag = 1;
	}else {
		for (int i = 0; i < Server->ClientCount; i++) {
			Server->Client[i]->Close();
			}
		Server->Close();
		Switcher->Stop();
		Form1->LocalPort->Enabled = this->EnableLocalPort;
		Form1->RemotePort->Enabled = true;
		Form1->RemoteAddr->Enabled = true;
		Form1->RealIP->Enabled = true;
		Form1->edWorker->Enabled = true;
		Form1->ComboBox1->Enabled = true;
		Form1->ComboBox2->Enabled = true;
		StartTime->Enabled = true;
		TrackBar1->Enabled = true;
		TrackBar2->Enabled = true;
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
Logic->GetSettings(Form1->ComboBox1->ItemIndex);
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
	case 2 :
		Form1->LocalPort->Text = "8008";
		break;
	case 3 :
		Form1->LocalPort->Text = "3355";
		break;
	;
}
Form1->RemoteAddr->Text = Logic->OSDProxyParams->RemoteAddress;
Form1->RemotePort->Text = Logic->OSDProxyParams->RemotePort;
Form1->RealIP->Text = Logic->OSDProxyParams->RemoteIP;
Form1->edWorker->Text = Logic->OSDProxyParams->Login;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
Logic = new TLogic(Form1->ComboBox1->ItemIndex);
Logic->GetSettings(Form1->ComboBox1->ItemIndex);
NetworkConfigs = new std::vector<TNetworkConfig>(0);
SslContext = new TSslContext((TComponent*)Form1);
Server = new TServer((TComponent*)Form1);
Server->SslContext = SslContext;
Server->ServerLog = Log;
Server->ServerLogic = Logic;
Logic->ApplySettings(Server);
Switcher = new TSwitcher(Form1);
Server->Init();
this->LoadFromFile();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
Form3->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadFromFile()
{
bool autostart = false;
char* buf = "";
UnicodeString bufstr;
ifstream fin("config.txt");
while (fin.is_open()&&!fin.eof()){
	fin >> (char*)buf;
	bufstr = buf;
	if (bufstr == "-localport") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->LocalPort->Text = bufstr;
		continue;
		}
	if (bufstr == "-domainaddress") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->RemoteAddr->Text = bufstr;
		continue;
		}
	if (bufstr == "-ipaddress") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->RealIP->Text = bufstr;
		continue;
		}
	if (bufstr == "-remoteport") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->RemotePort->Text = bufstr;
		continue;
		}
	if (bufstr == "-worker") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->edWorker->Text = bufstr;
		continue;
		}
	if (bufstr == "-loglevel") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->ComboBox2->ItemIndex = bufstr.ToInt();
		continue;
		}
	if (bufstr == "-minerversion") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->ComboBox1->ItemIndex = bufstr.ToInt();
		Logic->GetSettings(Form1->ComboBox1->ItemIndex);
		Form3->LoadNetworkSettings();
		Form3->FillAdaptersData();
		Form3->FillHostsData();
		Form3->FillNetworkData();
		continue;
		}
	if (bufstr == "-enablelocalport") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			this->EnableLocalPort = true;
			this->LocalPort->Enabled = true;
			}
		continue;
		}
	if (bufstr == "-proxyonly") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			Server->ServerLogic->SetProxyOnly(true);
			}
		continue;
		}
	if (bufstr == "-autostart") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			autostart = true;
			}
		continue;
		}
	if (bufstr == "-starttime") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->StartTime->Text = bufstr;
		continue;
		}
	if (bufstr == "-donationosd") {
		fin >> (char*)buf;
		bufstr = buf;
		if ((bufstr.ToInt() > 0)&&(bufstr.ToInt() < 25)) {
			Form1->TrackBar1->Position = bufstr.ToInt() -1;
			}
		continue;
		}
	if (bufstr == "-donationdev") {
		fin >> (char*)buf;
		bufstr = buf;
		if ((bufstr.ToInt() > 0)&&((bufstr.ToInt() + Form1->TrackBar1->Position) < 24)) {
			Form1->TrackBar2->Position = bufstr.ToInt();
			}
		continue;
		}
	}
fin.close();
if (autostart) {
    this->ListenBtnClick(Form1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
Form1->Label8->Caption = "Donation - "+IntToStr(Form1->TrackBar1->Position + 1)+" hours per day";
Form1->TrackBar2->Max = 24 - Form1->TrackBar1->Position - 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
Form1->Label9->Caption = "Donation to miner dev - "+IntToStr(Form1->TrackBar2->Position)+" hours";
}
//---------------------------------------------------------------------------

