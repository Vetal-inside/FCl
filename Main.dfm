object Form1: TForm1
  Left = 267
  Top = 290
  BorderStyle = bsSingle
  Caption = 'FCl project: Windows MITM proxy for proprietary miners'
  ClientHeight = 108
  ClientWidth = 953
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
  object Label7: TLabel
    Left = 447
    Top = 57
    Width = 17
    Height = 13
    Caption = 'Log'
  end
  object Label9: TLabel
    Left = 768
    Top = 57
    Width = 152
    Height = 13
    Caption = 'Donation to miner dev - 0 hours'
  end
  object Label8: TLabel
    Left = 768
    Top = 8
    Width = 129
    Height = 13
    Caption = 'Donation - 4 hours per day'
  end
  object Label10: TLabel
    Left = 536
    Top = 57
    Width = 47
    Height = 13
    Caption = 'Start time'
  end
  object RemotePort: TEdit
    Left = 447
    Top = 24
    Width = 75
    Height = 21
    TabOrder = 0
    Text = '3333'
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
  object edWorker: TEdit
    Left = 120
    Top = 76
    Width = 313
    Height = 21
    TabOrder = 3
    Text = 't1ZFCLi5vyXye2t6Xm68Kzukyi5kYo44JrB'
  end
  object RemoteAddr: TEdit
    Left = 120
    Top = 24
    Width = 217
    Height = 21
    TabOrder = 4
    Text = 'eu1-zcash.flypool.org'
  end
  object RealIP: TEdit
    Left = 352
    Top = 24
    Width = 81
    Height = 21
    TabOrder = 5
    Text = '94.23.60.113'
  end
  object ComboBox1: TComboBox
    Left = 536
    Top = 24
    Width = 217
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 6
    Text = 'Claymore'#39's ZCash miner v12.4+'
    OnChange = ComboBox1Change
    Items.Strings = (
      'Claymore'#39's ZCash miner v12.4+'
      'Claymore'#39's Ethereum miner v7.4'
      'Claymore'#39's CryptoNote Miner v9.7')
  end
  object Button1: TButton
    Left = 625
    Top = 76
    Width = 75
    Height = 21
    Caption = 'Log'
    TabOrder = 7
    OnClick = Button1Click
  end
  object ComboBox2: TComboBox
    Left = 447
    Top = 76
    Width = 75
    Height = 21
    Style = csDropDownList
    ItemIndex = 1
    TabOrder = 8
    Text = 'Short log'
    OnChange = ComboBox1Change
    Items.Strings = (
      'Without log'
      'Short log'
      'Full log')
  end
  object TrackBar1: TTrackBar
    Left = 768
    Top = 24
    Width = 161
    Height = 21
    Max = 24
    Position = 2
    TabOrder = 9
    ThumbLength = 15
    OnChange = TrackBar1Change
  end
  object TrackBar2: TTrackBar
    Left = 768
    Top = 76
    Width = 161
    Height = 21
    Max = 1
    TabOrder = 10
    ThumbLength = 15
    OnChange = TrackBar2Change
  end
  object StartTime: TEdit
    Left = 536
    Top = 76
    Width = 75
    Height = 21
    TabOrder = 11
    Text = '00:00'
  end
  object LocalPorts: TMemo
    Left = 16
    Top = 24
    Width = 75
    Height = 46
    Enabled = False
    Lines.Strings = (
      '3443'
      '6633'
      '2242')
    TabOrder = 2
  end
end
