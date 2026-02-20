// Stephen Johnston


#include "MP_LANMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

// Called when the widget is initialized
void UMP_LANMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set input mode to UI only (player cannot control character) and show the players cursor
	FInputModeUIOnly UIInputMode;
	GetOwningPlayer()->SetInputMode(UIInputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
	
	// Bind the host and join button clicks to their relative functions
	Button_Host->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	Button_Join->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
}

void UMP_LANMenu::HostButtonClicked()
{
	// Set input mode to game only (gives player control over character again) and hide the cursor
	FInputModeGameOnly GameInputMode;
	GetOwningPlayer()->SetInputMode(GameInputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	// Open a new listen server on the world HostingLevel
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, HostingLevel, true, TEXT("listen"));
}

void UMP_LANMenu::JoinButtonClicked()
{
	// Set input mode to game only (gives player control over character again) and hide the cursor
	FInputModeGameOnly GameInputMode;
	GetOwningPlayer()->SetInputMode(GameInputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	// Get the text (IP address) entered in TextBox_IPAddress
	const FString Address = TextBox_IpAddress->GetText().ToString();

	// Open the level at the IP address
	UGameplayStatics::OpenLevel(this, *Address);
}
