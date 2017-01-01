//---------------------------------------------------------------------------

#ifndef uLogH
#define uLogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TLog : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *LogWnd;
	TPopupMenu *PopupMenu;
	TMenuItem *Copy1;
	TMenuItem *Clear1;
	TMenuItem *Refresh1;
	TMenuItem *LineMode1;
	TMenuItem *Debugstring1;
	TMenuItem *Wordwrap1;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	void Add(UnicodeString ToLog);
	__fastcall TLog(TComponent* Owner);
	TColor TextColor;
	String Header;
};
UnicodeString RemoveLowChar(UnicodeString Source);
char _DebugStrLowChar(char);
//---------------------------------------------------------------------------
extern PACKAGE TLog *Log;
//---------------------------------------------------------------------------
#endif
