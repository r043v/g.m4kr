//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Tpreview : public TForm
{
__published:	// Composants gérés par l'EDI
        TImage *img;
        TScrollBar *frmSelect;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall frmSelectChange(TObject *Sender);
        void __fastcall imgClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
        void __fastcall updatePreviewBitmap(void);
        __fastcall Tpreview(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tpreview *preview;
//---------------------------------------------------------------------------
#endif
