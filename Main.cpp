//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

//---------------------------------------------------------------------------

#pragma link "OverbyteIcsWndControl"
#pragma link "OverbyteIcsWSocket"
#pragma link "OverbyteIcsWSocketS"
#pragma resource "*.dfm"

TForm1 *Form1;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ListenBtnClick(TObject *Sender)
{
if (ListenBtn->Tag == 0) {
	WSocketServer->Banner = "";
	WSocketServer->BannerTooBusy = "";
	WSocketServer->ClientClass = __classid(TClient);
	WSocketServer->Addr = "0.0.0.0";
	WSocketServer->Port = LocalPort->Text;
	WSocketServer->Listen();
	LocalPort->Enabled = false;
	RemotePort->Enabled = false;
	RemoteAddr->Enabled = false;
	RealIP->Enabled = false;
	edWorker->Enabled = false;
	ListenBtn->Caption = "Cancel";
	ListenBtn->Tag = 1;
	}else {
		for (int i = 0; i < WSocketServer->ClientCount; i++) {
			WSocketServer->Client[i]->Close();
			}
		  WSocketServer->Close();
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WSocketServerClientConnect(TObject *Sender, TWSocketClient *Client, WORD Error)
{
const TColor aColor[11]  = {clBlack, clMaroon, clGreen, clOlive, clNavy, clPurple, clRed, clLime, clYellow, clBlue, clFuchsia};
((TClient*)Client)->Rcvd = "";
Client->OnDataAvailable = ClientDataAvailable;
Client->OnBgException = BgException;
Client->LineMode = false;
Client->LineEnd = "\r\n";
((TClient*)Client)->RemoteSocket = new TWSocket(((TClient*)Client));
((TClient*)Client)->RemoteSocket->Port = Form1->RemotePort->Text;
((TClient*)Client)->RemoteSocket->LineMode = false;
((TClient*)Client)->RemoteSocket->LineEnd = "\r\n";
((TClient*)Client)->RemoteSocket->OnSessionConnected = RemoteSessionConnected;
((TClient*)Client)->RemoteSocket->OnDataAvailable = RemoteDataAvailable;
((TClient*)Client)->RemoteSocket->OnSessionClosed = RemoteSessionClosed;
((TClient*)Client)->RemoteSocket->OnBgException = BgException;
((TClient*)Client)->RemoteSocket->Addr = Form1->RealIP->Text;
((TClient*)Client)->RemoteSocket->Connect();
Form1->StatusBar->Panels->operator [](1)->Text = IntToStr(WSocketServer->ClientCount);
Form1->StatusBar->Panels->operator [](3)->Text = Client->GetPeerAddr();
if (WSocketServer->ClientCount <=11) {
	Log->Color = aColor[WSocketServer->ClientCount];
	} else {
		Color = clBlack;
		};
}

void __fastcall TForm1::RemoteSessionConnected(TObject *Sender, WORD Error)
{
if (Error != 0) {exit(Error);};
UnicodeString abc = Sender->ClassName();
abc = ((TWSocket*)Sender)->Owner->ClassName();
abc = ((TWSocket*)Sender)->ClassName();
if (Form1->CheckBox1->Checked) {
	Log->Header = "Connection Opened";
	Log->Add("");
	}
if (((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd != "") {
	if (Form1->CheckBox1->Checked) {
		Log->Header = "Open From Local 1";
		Log->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd = ExchangeString(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	if (Form1->CheckBox1->Checked) {
		Log->Header = "Open From Local 2";
		Log->Add(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
		}
	((TClient*)(((TWSocket*)Sender)->Owner))->RemoteSocket->SendStr(((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd);
	((TClient*)(((TWSocket*)Sender)->Owner))->Rcvd="";
	}
}

void __fastcall TForm1::RemoteDataAvailable(TObject *Sender, WORD Error)
{
UnicodeString FromRemote;
if (Error != 0) {exit(Error);};
FromRemote = ((TClient*)(((TWSocket*)Sender)))->ReceiveStr();
if (FromRemote != "") {
	if (Form1->CheckBox1->Checked) {
		Log->Header = "From Remote";
		Log->Add(FromRemote);
		}
	if (((TClient*)(((TWSocket*)Sender)->Owner))->State == wsConnected) {
		((TClient*)(((TWSocket*)Sender)->Owner))->SendStr(FromRemote);
		} else {
			if (Form1->CheckBox1->Checked) {
				Log->Header = "Error: Local has closed";
				Log->Add("");
				}
			((TClient*)(((TWSocket*)Sender)->Owner))->CloseDelayed();
			};
	}
}

UnicodeString __fastcall TForm1::ExchangeString(UnicodeString si)
{
UnicodeString s;
s = si;
s = StringReplace(s, "\"zec-eu1.nanopool.org\"", "\""+RemoteAddr->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"us1-zcash.flypool.org\"", "\""+RemoteAddr->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"eu1-zcash.flypool.org\"", "\""+RemoteAddr->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s, "\"zec.suprnova.cc\"", "\""+RemoteAddr->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));

s = StringReplace(s, "\""+LocalPort->Text+"\"", "\""+RemotePort->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));

s = StringReplace(s,"\"t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr\"","\""+edWorker->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t\"","\""+edWorker->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv\"","\""+edWorker->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP\"","\""+edWorker->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
s = StringReplace(s,"\"1LmMNkiEvjapn5PRY8A9wypcWJveRrRGWr\"","\""+edWorker->Text+"\"",(TReplaceFlags)(TReplaceFlags()<< rfReplaceAll << rfIgnoreCase));
return s;
}

void __fastcall TForm1::ClientDataAvailable(TObject *Sender, WORD Error)
{
if (Error != 0) {exit(Error);};
((TClient*)Sender)->Rcvd = ((TClient*)Sender)->Rcvd + ((TClient*)Sender)->ReceiveStr();
if ((((TClient*)Sender)->RemoteSocket->State == wsConnected) &&(((TClient*)Sender)->Rcvd != "")) {
	if (Form1->CheckBox1->Checked) {
		Log->Header = "From Local 1";
		Log->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->Rcvd = ExchangeString(((TClient*)Sender)->Rcvd);
	if (Form1->CheckBox1->Checked) {
		Log->Header = "From Local 2";
		Log->Add(((TClient*)Sender)->Rcvd);
		}
	((TClient*)Sender)->RemoteSocket->SendStr(((TClient*)Sender)->Rcvd);
	((TClient*)Sender)->Rcvd = "";
	}
}

void __fastcall TForm1::WSocketServerClientDisconnect(TObject *Sender, TWSocketClient *Client,
		  WORD Error)
{
Form1->StatusBar->Panels->operator [](1)->Text = IntToStr(((TWSocketServer*)(((TClient*)Client)->Server))->ClientCount -1);
if (((TWSocketServer*)(((TClient*)Client)->Server))->ClientCount > 1) {
	Form1->StatusBar->Panels->operator [](3)->Text = ((TWSocketServer*)(((TClient*)Client)->Server))->Client[((TWSocketServer*)(((TClient*)Client)->Server))->ClientCount - 1]->GetPeerAddr();
	} else {
		Form1->StatusBar->Panels->operator [](3)->Text = "";
		}
}

void __fastcall TForm1::RemoteSessionClosed(TObject *Sender, WORD Error)
{
((TClient*)(((TWSocket*)Sender)->Owner))->Shutdown(1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BgException(TObject *Sender, Exception *E, bool &CanClose)

{
CanClose = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WSocketServerSessionClosed(TObject *Sender, WORD ErrCode)

{
Form1->LocalPort->Enabled = true;
Form1->RemotePort->Enabled = true;
Form1->RemoteAddr->Enabled = true;
Form1->RealIP->Enabled = true;
Form1->edWorker->Enabled = true;
Form1->ListenBtn->Caption = "Listen";
Form1->ListenBtn->Tag = 0;
}
//---------------------------------------------------------------------------



