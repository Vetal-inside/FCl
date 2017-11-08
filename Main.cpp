//---------------------------------------------------------------------------
#include <vcl.h>
#include <fstream>
#include <stdio.h>
#include <io.h>
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
std::vector<TServer*>* Servers;
TLogic* Logic;
TSwitcher* Switcher;
TDivert* Divert;
std::vector<UnicodeString>* VectLocalPorts;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
this->EnableLocalPort = false;
this->minOSDDonation = 2;
this->TrackBar1->Max = 24 - this->minOSDDonation;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenBtnClick(TObject *Sender)
{
int i,j;
if (ListenBtn->Tag == 0) {
	for (int i = 0; i < Logic->countServers; i++) {
		Servers->operator [](i)->Init();
		Servers->operator [](i)->SetLogLevel(Form1->ComboBox2->ItemIndex);
		};
	Logic->GetSettings(Form1->ComboBox1->ItemIndex);
	Logic->ApplySettings(Servers);
	for (i = 0; i < Form1->LocalPorts->Lines->Count; i++) {
		VectLocalPorts->resize(VectLocalPorts->size() + 1);
		VectLocalPorts->operator [](i) = Form1->LocalPorts->Lines->operator [](i);
		}
	Logic->GetNProxyParams(VectLocalPorts,this->RemotePort->Text,this->RealIP->Text,this->RemoteAddr->Text,this->edWorker->Text);
	Logic->ApplyProxyParams(Servers,2);
	Switcher->Init(Form1->TrackBar1->Position + Form1->minOSDDonation,Form1->TrackBar2->Position,Servers,Logic,Form1->StartTime->Text);
	Switcher->Start();
	Divert = new TDivert(true);
	Divert->Init(Logic, Log);
	Divert->Start();
	for (int i = 0; i < Logic->countServers; i++) {
		Servers->operator [](i)->Listen();
		};
	LocalPorts->Enabled = false;
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
	ListenBtn->Caption = "Cancel";
	ListenBtn->Tag = 1;
	}else {
		for (i = 0; i < Logic->countServers; i++){
			for (j = 0; j < Servers->operator [](i)->ClientCount; j++) {
				Servers->operator [](i)->Client[j]->Close();
				};
			Servers->operator [](i)->Close();
			};
		Divert->Needed = false;
		Sleep(500);
		Divert->Terminate();
		Switcher->Stop();
		Form1->LocalPorts->Enabled = this->EnableLocalPort;
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
Form1->LocalPorts->Lines->Clear();
for (unsigned int i = 0; i < Logic->OSDProxyParams->LocalPorts->size(); i++) {
	Form1->LocalPorts->Lines->Add(Logic->OSDProxyParams->LocalPorts->operator [](i));
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
VectLocalPorts = new std::vector<UnicodeString>(0);
Servers = new std::vector<TServer*>;
Servers->resize(Logic->countServers);
for (int i = 0; i < Logic->countServers; i++) {
	Servers->operator [](i) = new TServer((TComponent*)Form1);
	Servers->operator [](i)->SslContext = new TSslContext((TComponent*)Form1);
	Servers->operator [](i)->ServerLog = Log;
	Servers->operator [](i)->ServerLogic = Logic;
	};
Logic->ApplySettings(Servers);
Switcher = new TSwitcher(Form1);
for (int i = 0; i < Logic->countServers; i++) {
	Servers->operator [](i)->Init();
	};
this->LoadFromFile();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadFromFile()
{
bool autostart = false;
bool firstlocalport = true;
char* buf = "";
UnicodeString bufstr;
ifstream fin("config.txt");
while (fin.is_open()&&!fin.eof()){
	fin >> (char*)buf;
	bufstr = buf;
	if (bufstr == "-localport") {
		fin >> (char*)buf;
		bufstr = buf;
		if (firstlocalport) {
			Form1->LocalPorts->Lines->Clear();
			firstlocalport = false;
			};
		Form1->LocalPorts->Lines->Add(bufstr);
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
	if (bufstr == "-divertlog") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			Logic->SetDivertLog(1);
			}
		continue;
		}
	if (bufstr == "-minerversion") {
		fin >> (char*)buf;
		bufstr = buf;
		Form1->ComboBox1->ItemIndex = bufstr.ToInt();
		Logic->GetSettings(Form1->ComboBox1->ItemIndex);
		continue;
		}
	if (bufstr == "-enablelocalport") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			this->EnableLocalPort = true;
			this->LocalPorts->Enabled = true;
			}
		continue;
		}
	if (bufstr == "-proxyonly") {
		fin >> (char*)buf;
		bufstr = buf;
		if (bufstr == "1") {
			Logic->SetProxyOnly(true);
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
		if ((bufstr.ToInt() >= this->minOSDDonation)&&(bufstr.ToInt() < 25)) {
			Form1->TrackBar1->Position = bufstr.ToInt() - this->minOSDDonation;
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
Form1->Label8->Caption = "Donation - "+IntToStr(Form1->TrackBar1->Position + Form1->minOSDDonation)+" hours per day";
Form1->TrackBar2->Max = 24 - Form1->TrackBar1->Position - Form1->minOSDDonation;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
Form1->Label9->Caption = "Donation to miner dev - "+IntToStr(Form1->TrackBar2->Position)+" hours";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
Form2->Show();
}
//---------------------------------------------------------------------------

