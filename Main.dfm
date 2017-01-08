object Form1: TForm1
  Left = 267
  Top = 290
  BorderStyle = bsSingle
  Caption = 'FCl project: Windows MITM proxy for proprietary miners'
  ClientHeight = 108
  ClientWidth = 721
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCanResize = FormCanResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 47
    Height = 13
    Caption = 'Local port'
  end
  object Label2: TLabel
    Left = 120
    Top = 8
    Width = 76
    Height = 13
    Caption = 'Domain address'
  end
  object Label3: TLabel
    Left = 352
    Top = 8
    Width = 51
    Height = 13
    Caption = 'IP address'
  end
  object Label4: TLabel
    Left = 447
    Top = 8
    Width = 60
    Height = 13
    Caption = 'Remote port'
  end
  object Label5: TLabel
    Left = 120
    Top = 57
    Width = 106
    Height = 13
    Caption = 'Replace wallet/worker'
  end
  object Label6: TLabel
    Left = 536
    Top = 8
    Width = 64
    Height = 13
    Caption = 'Miner version'
  end
  object RemotePort: TEdit
    Left = 447
    Top = 24
    Width = 75
    Height = 21
    TabOrder = 0
    Text = '3357'
  end
  object ListenBtn: TButton
    Left = 16
    Top = 76
    Width = 75
    Height = 21
    Caption = 'Listen'
    TabOrder = 1
    OnClick = ListenBtnClick
  end
  object CheckBox1: TCheckBox
    Left = 447
    Top = 78
    Width = 75
    Height = 17
    Caption = 'Logs'
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
  object LocalPort: TEdit
    Left = 16
    Top = 24
    Width = 75
    Height = 21
    TabOrder = 3
    Text = '3333'
  end
  object edWorker: TEdit
    Left = 120
    Top = 76
    Width = 298
    Height = 21
    TabOrder = 4
    Text = '12enkHEmDsF1e7jwyXZY2DdqdJNNEnRpvA'
  end
  object RemoteAddr: TEdit
    Left = 120
    Top = 24
    Width = 217
    Height = 21
    TabOrder = 5
    Text = 'equihash.eu.nicehash.com'
  end
  object RealIP: TEdit
    Left = 352
    Top = 24
    Width = 81
    Height = 21
    TabOrder = 6
    Text = '5.153.50.217'
  end
  object ComboBox1: TComboBox
    Left = 536
    Top = 24
    Width = 169
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 7
    Text = 'Claymore'#39's ZCash miner v9.1'
    OnChange = ComboBox1Change
    Items.Strings = (
      'Claymore'#39's ZCash miner v9.1'
      'Claymore'#39's ZCash miner v9.3'
      'Claymore'#39's ZCash miner v10.0')
  end
  object Button1: TButton
    Left = 536
    Top = 76
    Width = 75
    Height = 21
    Caption = 'Settings'
    TabOrder = 8
    OnClick = Button1Click
  end
end
