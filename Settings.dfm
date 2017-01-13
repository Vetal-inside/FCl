object Form3: TForm3
  Left = 119
  Top = 442
  Caption = 'Network confuguration'
  ClientHeight = 372
  ClientWidth = 1049
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 216
    Top = 5
    Width = 144
    Height = 13
    Caption = 'Choose your network adapter'
  end
  object Label1: TLabel
    Left = 6
    Top = 5
    Width = 137
    Height = 26
    Caption = 'Add this to C:\Windows \System32\drivers\etc\hosts'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 216
    Top = 51
    Width = 267
    Height = 13
    Caption = 'Run cmd.exe with administrator privileges and execute:'
  end
  object Label3: TLabel
    Left = 641
    Top = 5
    Width = 26
    Height = 13
    Caption = 'Rig #'
  end
  object ComboBox4: TComboBox
    Left = 216
    Top = 24
    Width = 410
    Height = 21
    Style = csDropDownList
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 0
    OnChange = ComboBox4Change
  end
  object Memo1: TMemo
    Left = 6
    Top = 38
    Width = 198
    Height = 326
    TabOrder = 1
  end
  object Memo2: TMemo
    Left = 216
    Top = 70
    Width = 793
    Height = 294
    TabOrder = 2
    WordWrap = False
  end
  object ComboBox1: TComboBox
    Left = 641
    Top = 24
    Width = 56
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 3
    Text = '1'
    OnChange = ComboBox1Change
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10')
  end
end
