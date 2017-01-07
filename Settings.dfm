object Form3: TForm3
  Left = 30
  Top = 137
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
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 3
    Top = 5
    Width = 64
    Height = 13
    Caption = 'Miner version'
  end
  object Label4: TLabel
    Left = 216
    Top = 5
    Width = 144
    Height = 13
    Caption = 'Choose your network adapter'
  end
  object Label1: TLabel
    Left = 3
    Top = 51
    Width = 137
    Height = 26
    Caption = 'Add this to C:\Windows \System32\drivers\etc\hosts'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 216
    Top = 51
    Width = 274
    Height = 13
    Caption = 'Execute this in command line with administrator privileges'
  end
  object ComboBox3: TComboBox
    Left = 3
    Top = 24
    Width = 198
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = 'Claymore'#39's ZCash miner v9.1'
    Items.Strings = (
      'Claymore'#39's ZCash miner v9.1'
      'Claymore'#39's ZCash miner v9.3')
  end
  object ComboBox4: TComboBox
    Left = 216
    Top = 24
    Width = 410
    Height = 21
    Style = csDropDownList
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 1
    OnChange = ComboBox4Change
  end
  object Memo1: TMemo
    Left = 3
    Top = 80
    Width = 198
    Height = 284
    Lines.Strings = (
      '8.8.8.100  equihash.eu.nicehash.com'
      '8.8.8.101  equihash.eu.nicehash.col'
      '8.8.8.102  us1-zcash.flypool.org'
      '8.8.8.103  eu1-zcash.flypool.org'
      '8.8.8.104  zec-eu1.nanopool.org'
      '8.8.8.105  zec.suprnova.cc')
    TabOrder = 2
  end
  object Memo2: TMemo
    Left = 216
    Top = 80
    Width = 793
    Height = 284
    TabOrder = 3
    WordWrap = False
  end
end
