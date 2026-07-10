#include "MyGameViewport.h"

void UMyGameViewport::Activated(FViewport* InViewport, const FWindowActivateEvent& ActivateEvent) 
{
    // Store the focused widget before calling super
    TSharedPtr<SWidget> FocusedWidget = FSlateApplication::Get().GetKeyboardFocusedWidget();

    Super::Activated(InViewport, ActivateEvent);

    // Restore it after super potentially clears it
    if (FocusedWidget.IsValid())
    {
        FSlateApplication::Get().SetKeyboardFocus(FocusedWidget);
    }
}