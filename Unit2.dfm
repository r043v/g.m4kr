object preview: Tpreview
  Left = 504
  Top = 444
  Width = 194
  Height = 179
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = ' GfMaker .42'
  Color = clBtnFace
  TransparentColorValue = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefaultPosOnly
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object img: TImage
    Left = 2
    Top = 0
    Width = 184
    Height = 150
    Center = True
    DragCursor = crNoDrop
    DragKind = dkDock
    IncrementalDisplay = True
    ParentShowHint = False
    Proportional = True
    ShowHint = False
    Stretch = True
    Visible = False
    OnClick = imgClick
  end
  object frmSelect: TScrollBar
    Left = 0
    Top = -1
    Width = 9
    Height = 152
    Kind = sbVertical
    PageSize = 0
    TabOrder = 0
    Visible = False
    OnChange = frmSelectChange
  end
end
