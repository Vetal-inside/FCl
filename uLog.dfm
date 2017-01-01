object Log: TLog
  Left = 0
  Top = 0
  Caption = 'Log'
  ClientHeight = 537
  ClientWidth = 1111
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object LogWnd: TRichEdit
    Left = 40
    Top = 16
    Width = 185
    Height = 89
    Color = clBtnFace
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'LogWnd')
    ParentFont = False
    PopupMenu = PopupMenu
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object PopupMenu: TPopupMenu
    Left = 56
    Top = 128
    object Copy1: TMenuItem
      Caption = '&Copy'
    end
    object Clear1: TMenuItem
      Caption = '&Clear'
    end
    object Refresh1: TMenuItem
      Caption = '&Refresh'
      Checked = True
    end
    object LineMode1: TMenuItem
      Caption = '&LineMode'
    end
    object Debugstring1: TMenuItem
      Caption = '&Debugstring'
    end
    object Wordwrap1: TMenuItem
      Caption = '&Word wrap'
    end
  end
end
