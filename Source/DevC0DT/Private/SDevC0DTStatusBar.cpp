// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDevC0DTStatusBar.h"
#include "EditorDevC0DTUtilitiesStyle.h"

#include "CoreGlobals.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/FileManager.h"
#include "Logging/MessageLog.h"
#include "Logging/TokenizedMessage.h"
#include "Math/Color.h"
#include "MessageLogModule.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
//#include "ProfilingDebugging/MiscTrace.h"
//#include "ProfilingDebugging/TraceScreenshot.h"
//#include "SRecentTracesList.h"
#include "Styling/StyleColors.h"
#include "ToolMenus.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "InsightsEditor"

const TCHAR* SDevC0DTStatusBarWidget::DefaultPreset = TEXT("default");
const TCHAR* SDevC0DTStatusBarWidget::MemoryPreset= TEXT("default,memory");
const TCHAR* SDevC0DTStatusBarWidget::TaskGraphPreset = TEXT("default,task");
const TCHAR* SDevC0DTStatusBarWidget::ContextSwitchesPreset = TEXT("default,contextswitches");

const TCHAR* SDevC0DTStatusBarWidget::SettingsCategory = TEXT("EditorTraceUtilities");
const TCHAR* SDevC0DTStatusBarWidget::OpenLiveSessionOnTraceStartSettingName = TEXT("OpenLiveSessionOnTraceStart");
const TCHAR* SDevC0DTStatusBarWidget::OpenInsightsAfterTraceSettingName = TEXT("OpenInsightsAfterTrace");
const TCHAR* SDevC0DTStatusBarWidget::ShowInExplorerAfterTraceSettingName = TEXT("ShowInExplorerAfterTrace");

class FOpenLiveSessionTask
{
public:
	FOpenLiveSessionTask()
	{}

	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FOpenLiveSessionTask, STATGROUP_TaskGraphTasks); }
	ENamedThreads::Type GetDesiredThread() { return ENamedThreads::Type::GameThread; }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		//FUnrealInsightsLauncher::Get()->TryOpenTraceFromDestination(FTraceAuxiliary::GetTraceDestinationString());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// FInsightsStatusBarWidgetCommands
////////////////////////////////////////////////////////////////////////////////////////////////////

//class FInsightsStatusBarWidgetCommands : public TCommands<FInsightsStatusBarWidgetCommands>
//{
//public:
//	FInsightsStatusBarWidgetCommands()
//		: TCommands<FInsightsStatusBarWidgetCommands>(
//			TEXT("InsightsStatusBarWidgetCommands"),
//			NSLOCTEXT("Contexts", "InsightsStatusBarWidgetCommands", "Insights Status Bar"),
//			NAME_None,
//			FEditorTraceUtilitiesStyle::Get().GetStyleSetName())
//	{
//	}
//
//	virtual ~FInsightsStatusBarWidgetCommands()
//	{
//	}
//
//	// UI_COMMAND takes long for the compiler to optimize
//	UE_DISABLE_OPTIMIZATION_SHIP
//	virtual void RegisterCommands() override
//	{
//		UI_COMMAND(Command_TraceScreenshot, "Trace Screenshot", "Takes a screenshot and sends it to the trace.", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::F9));
//		UI_COMMAND(Command_TraceBookmark, "Trace Bookmark", "Traces a bookmark.", EUserInterfaceActionType::Button, FInputChord());
//	}
//	UE_ENABLE_OPTIMIZATION_SHIP
//
//	TSharedPtr<FUICommandInfo> Command_TraceScreenshot;
//	TSharedPtr<FUICommandInfo> Command_TraceBookmark;
//};

TSharedRef<SWidget> CreateC0DTStatusBarWidget()
{
	return SNew(SDevC0DTStatusBarWidget);
}

void RegisterC0DTStatusWidgetWithToolMenu()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.StatusBar.ToolBar"));

	FToolMenuSection& C0DTSection = Menu->AddSection(TEXT("C0DT"), FText::GetEmpty(), FToolMenuInsert(NAME_None, EToolMenuInsertType::First));

	C0DTSection.AddEntry(
		FToolMenuEntry::InitWidget(TEXT("C0DTStatusBar"), CreateC0DTStatusBarWidget(), FText::GetEmpty(), true, false)
	);
}

void SDevC0DTStatusBarWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SDevC0DTStatusBarWidget::Construct(const FArguments& InArgs)
{
	const FSlateBrush* MainStatusBarIcon = FEditorDevC0DTUtilitiesStyle::Get().GetBrush("Icons.MainIcon.StatusBar");

	this->ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SComboButton)
			.ContentPadding(FMargin(6.0f, 0.0f))
			.MenuPlacement(MenuPlacement_AboveAnchor)
			.ComboButtonStyle(&FAppStyle::Get().GetWidgetStyle<FComboButtonStyle>("SimpleComboButton"))
			.OnGetMenuContent(this, &SDevC0DTStatusBarWidget::MakeMenuContent)
			.HasDownArrow(true)
			.ToolTipText(this, &SDevC0DTStatusBarWidget::GetTitleToolTipText)
			.ButtonContent()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(0, 0, 3, 0)
				.AutoWidth()
				[
					SNew(SImage)
					.ColorAndOpacity(FSlateColor::UseForeground())
					.Image(MainStatusBarIcon)
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("C0DT", "DevTools"))
					.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("DialogButtonText"))
				]
			]
		]

	//	+ SHorizontalBox::Slot()
	//	.AutoWidth()
	//	[
	//		SNew(SButton)
	//		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
	//		.ContentPadding(FMargin(0.0f, 0.0f, 0.0f, 3.0f))
	//		.HAlign(HAlign_Left)
	//		.VAlign(VAlign_Bottom)
	//		.ToolTipText(this, &SDevC0DTStatusBarWidget::GetRecordingButtonTooltipText)
	//		.OnClicked_Lambda([this]() { this->ToggleTracing_OnClicked(); return FReply::Handled(); })
	//		.OnHovered_Lambda([this]() { this->bIsTraceRecordButtonHovered = true; })
	//		.OnUnhovered_Lambda([this]() { this->bIsTraceRecordButtonHovered = false; })
	//		.Content()
	//		[
	//			SNew(SOverlay)

	//			+ SOverlay::Slot()
	//			[
	//				SNew(SImage)
	//				.ColorAndOpacity(this, &SDevC0DTStatusBarWidget::GetRecordingButtonColor)
	//				.Image(FEditorTraceUtilitiesStyle::Get().GetBrush("Icons.RecordTraceCenter.StatusBar"))
	//				.Visibility(this, &SDevC0DTStatusBarWidget::GetStartTraceIconVisibility)
	//			]

	//			+ SOverlay::Slot()
	//			[
	//				SNew(SImage)
	//				.ColorAndOpacity(this, &SDevC0DTStatusBarWidget::GetRecordingButtonOutlineColor)
	//				.Image(FEditorTraceUtilitiesStyle::Get().GetBrush("Icons.RecordTraceOutline.StatusBar"))
	//				.Visibility(this, &SDevC0DTStatusBarWidget::GetStartTraceIconVisibility)
	//			]

	//			+ SOverlay::Slot()
	//			[
	//				SNew(SImage)
	//				.Image(FEditorTraceUtilitiesStyle::Get().GetBrush("Icons.RecordTraceStop.StatusBar"))
	//				.Visibility(this, &SDevC0DTStatusBarWidget::GetStopTraceIconVisibility)
	//			]
	//		]
	//	]

	//	+ SHorizontalBox::Slot()
	//	.AutoWidth()
	//	[
	//		SNew(SButton)
	//		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
	//		.ContentPadding(FMargin(4.0f, 0.0f, 0.0f, 3.0f))
	//		.HAlign(HAlign_Left)
	//		.VAlign(VAlign_Bottom)
	//		.OnClicked_Lambda([this]() { SaveSnapshot(); return FReply::Handled(); })
	//		.Content()
	//		[
	//			SNew(SImage)
	//			.DesiredSizeOverride(CoreStyleConstants::Icon16x16)
	//			.Image(FEditorTraceUtilitiesStyle::Get().GetBrush("Icons.TraceSnapshot.StatusBar"))
	//			.ToolTipText(LOCTEXT("SaveSnapShot","Snapshot: Save Current Trace Buffer to active destination."))
	//		]
	//	]
	];

	//if (FTraceAuxiliary::GetConnectionType() == FTraceAuxiliary::EConnectionType::Network)
	//{
	//	TraceDestination = ETraceDestination::TraceStore;
	//}
	//if (FTraceAuxiliary::GetConnectionType() == FTraceAuxiliary::EConnectionType::File)
	//{
	//	TraceDestination = ETraceDestination::File;
	//}

	//LogListingName = TEXT("UnrealInsights");
	//FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	//if (!MessageLogModule.IsRegisteredLogListing(LogListingName))
	//{
	//	MessageLogModule.RegisterLogListing(LogListingName, LOCTEXT("UnrealInsights", "Unreal Insights"));
	//}

	//FTraceAuxiliary::OnTraceStarted.AddSP(this, &SDevC0DTStatusBarWidget::OnTraceStarted);
	//FTraceAuxiliary::OnTraceStopped.AddSP(this, &SDevC0DTStatusBarWidget::OnTraceStopped);
	//FTraceAuxiliary::OnSnapshotSaved.AddSP(this, &SDevC0DTStatusBarWidget::OnSnapshotSaved);

	//LiveSessionTracker = MakeShared<FLiveSessionTracker>();

	InitCommandList();
}

TSharedRef<SWidget> SDevC0DTStatusBarWidget::MakeMenuContent()
{
	//LiveSessionTracker->StartQuery();

	FMenuBuilder MenuBuilder(true, CommandList.ToSharedRef());

	MenuBuilder.BeginSection("TraceData", LOCTEXT("TraceMenu_Section_Data", "Trace Data"));
	{
		MenuBuilder.AddSubMenu
		(
			LOCTEXT("Channels", "Channels"),
			LOCTEXT("Channels_Desc", "Select what trace channels to enable when tracing."),
			FNewMenuDelegate::CreateSP(this, &SDevC0DTStatusBarWidget::Channels_BuildMenu),
			false,
			FSlateIcon("ActorPaletteStyle", ("Icons.Trace.Menu"))
		);

	/*	MenuBuilder.AddSubMenu
		(
			LOCTEXT("Channels", "Channels"),
			LOCTEXT("Channels_Desc", "Select what trace channels to enable when tracing."),
			FNewMenuDelegate::CreateSP(this, &SDevC0DTStatusBarWidget::Channels_BuildMenu),
			false,
			FSlateIcon("ActorPaletteStyle", ("Icons.Trace.Menu"))
		);

		MenuBuilder.AddMenuEntry
		(
			FInsightsStatusBarWidgetCommands::Get().Command_TraceScreenshot,
			NAME_None,
			TAttribute<FText>(),
			TAttribute<FText>(),
			FSlateIcon("ActorPaletteStyle", "Icons.Screenshot.Menu")
		);

		MenuBuilder.AddMenuEntry
		(
			FInsightsStatusBarWidgetCommands::Get().Command_TraceBookmark,
			NAME_None,
			TAttribute<FText>(),
			TAttribute<FText>(),
			FSlateIcon("ActorPaletteStyle", "Icons.Bookmark.Menu")
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("StatNamedEventsLabel", "Stat Named Events"),
			LOCTEXT("StatNamedEventsDesc", "Enable or disable named events in the stats system."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda([]() { GCycleStatsShouldEmitNamedEvents = GCycleStatsShouldEmitNamedEvents == 0 ? 1 : 0; }),
					  FCanExecuteAction::CreateLambda([]() { return true; }),
					  FIsActionChecked::CreateLambda([]() { return GCycleStatsShouldEmitNamedEvents > 0; })),
			NAME_None,
			EUserInterfaceActionType::ToggleButton
		);*/
	}
	MenuBuilder.EndSection();

	//MenuBuilder.BeginSection("TraceDestination", LOCTEXT("TraceMenu_Section_Destination", "Trace Destination"));
	//{
	///*	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("ServerLabel", "Trace Store"),
	//		LOCTEXT("ServerLabelDesc", "Set the trace store as the trace destination."),
	//		FSlateIcon(),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_Execute, ETraceDestination::TraceStore),
	//				  FCanExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_CanExecute),
	//				  FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_IsChecked, ETraceDestination::TraceStore)),
	//		NAME_None,
	//		EUserInterfaceActionType::RadioButton
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("FileLabel", "File"),
	//		LOCTEXT("FileLabelDesc", "Set file as the trace destination."),
	//		FSlateIcon(),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_Execute, ETraceDestination::File),
	//				  FCanExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_CanExecute),
	//				  FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::SetTraceDestination_IsChecked, ETraceDestination::File)),
	//		NAME_None,
	//		EUserInterfaceActionType::RadioButton
	//	);*/
	//}
	//MenuBuilder.EndSection();

	//MenuBuilder.BeginSection("Tracing", LOCTEXT("TraceMenu_Section_Tracing", "Tracing"));
	//{
	//	/*MenuBuilder.AddMenuEntry(
	//		TAttribute<FText>::CreateSP(this, &SDevC0DTStatusBarWidget::GetTraceMenuItemText),
	//		TAttribute<FText>::CreateSP(this, &SDevC0DTStatusBarWidget::GetTraceMenuItemTooltipText),
	//		FSlateIcon(FEditorTraceUtilitiesStyle::Get().GetStyleSetName(), "Icons.StartTrace.Menu"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleTracing_OnClicked)),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("SaveSnapshotLabel", "Save Trace Snapshot"),
	//		LOCTEXT("SaveSnapshotTooltip", "Save the current trace buffer to the selected trace destination."),
	//		FSlateIcon(FEditorTraceUtilitiesStyle::Get().GetStyleSetName(), "Icons.TraceSnapshot.Menu"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SaveSnapshot),
	//				  FCanExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::SaveSnapshot_CanExecute)),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);*/
	//}
	//MenuBuilder.EndSection();

	//MenuBuilder.BeginSection("Options", LOCTEXT("TraceMenu_Section_Options", "Options"));
	//{
	//	/*MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenLiveSesssionOnTraceStart", "Open Live Session on Trace Start"),
	//		LOCTEXT("OpenLiveSesssionOnTraceStartDesc", "When set, the live session will be automatically opened in Unreal Insights when tracing is started.\nThis option will only apply when tracing to the trace store."),
	//		FSlateIcon(),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleBooleanSettingValue, OpenLiveSessionOnTraceStartSettingName),
	//			FCanExecuteAction::CreateLambda([]() { return true; }),
	//			FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::GetBooleanSettingValue, OpenLiveSessionOnTraceStartSettingName)),
	//		NAME_None,
	//		EUserInterfaceActionType::ToggleButton
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenInsightsAfterTrace", "Open Insights after Trace"),
	//		LOCTEXT("OpenInsightsAfterTraceDesc", "When set, the session will be automatically opened in Unreal Insights when tracing is stopped or when a snapshot is saved."),
	//		FSlateIcon(),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleBooleanSettingValue, OpenInsightsAfterTraceSettingName),
	//			FCanExecuteAction::CreateLambda([]() { return true; }),
	//			FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::GetBooleanSettingValue, OpenInsightsAfterTraceSettingName)),
	//		NAME_None,
	//		EUserInterfaceActionType::ToggleButton
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("ShowInExplorerAfterTrace", "Show in Explorer after Trace"),
	//		LOCTEXT("ShowInExplorerAfterTraceDesc", "When set, folder containing the recorded session will be opened automatically when trace is stopped or when a snapshot is saved."),
	//		FSlateIcon(),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleBooleanSettingValue, ShowInExplorerAfterTraceSettingName),
	//			FCanExecuteAction::CreateLambda([]() { return true; }),
	//			FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::GetBooleanSettingValue, ShowInExplorerAfterTraceSettingName)),
	//		NAME_None,
	//		EUserInterfaceActionType::ToggleButton
	//	);*/
	//}
	//MenuBuilder.EndSection();

	//MenuBuilder.BeginSection("Locations", LOCTEXT("TraceMenu_Section_Locations", "Locations"));
	//{
	//	/*MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenTraceStoreLabel", "Open Trace Store Directory"),
	//		LOCTEXT("OpenTraceStoreTooltip", "Open Trace Store Directory. This is the location where traces saved to the trace server are stored."),
	//		FSlateIcon(FAppStyle::Get().GetStyleSetName(), "Icons.FolderOpen"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::OpenTraceStoreDirectory_OnClicked)),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenProfilingDirectoryLabel", "Open Profiling Directory"),
	//		LOCTEXT("OpenProfilingDirectoryTooltip", "Opens the profiling directory of the current project. This is the location where traces to file are stored."),
	//		FSlateIcon(FAppStyle::Get().GetStyleSetName(), "Icons.FolderOpen"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::OpenProfilingDirectory_OnClicked)),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);*/
	//}
	//MenuBuilder.EndSection();

	//MenuBuilder.BeginSection("Insights", LOCTEXT("TraceMenu_Section_Insights", "Insights"));
	//{
	///*	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenInsightsLabel", "Unreal Insights (Session Browser)"),
	//		LOCTEXT("OpenInsightsTooltip", "Launch the Unreal Insights Session Browser."),
	//		FSlateIcon(FEditorTraceUtilitiesStyle::Get().GetStyleSetName(), "Icons.UnrealInsights.Menu"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::LaunchUnrealInsights_OnClicked)),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);

	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("OpenLiveSessionLabel", "Open Live Session"),
	//		LOCTEXT("OpenLiveSessionTooltip", "Opening the live session is possible only while tracing to the trace store."),
	//		FSlateIcon(FEditorTraceUtilitiesStyle::Get().GetStyleSetName(), "Icons.OpenLiveSession.Menu"),
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::OpenLiveSession_OnClicked),
	//			FCanExecuteAction::CreateLambda([this]() { return FTraceAuxiliary::GetConnectionType() == FTraceAuxiliary::EConnectionType::Network; })),
	//		NAME_None,
	//		EUserInterfaceActionType::Button
	//	);

	//	MenuBuilder.AddSubMenu
	//	(
	//		LOCTEXT("RecentTracesLabel", "Recent Traces"),
	//		LOCTEXT("RecentTracesTooltop", "Open the latest traces recorded to the trace store or as a file."),
	//		FNewMenuDelegate::CreateSP(this, &SDevC0DTStatusBarWidget::Traces_BuildMenu),
	//		false,
	//		FSlateIcon(FEditorTraceUtilitiesStyle::Get().GetStyleSetName(), ("Icons.Trace.Menu"))
	//	);*/
	//}
	//MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void SDevC0DTStatusBarWidget::Channels_BuildMenu(FMenuBuilder& MenuBuilder)
{
	CreateChannelsInfo();

	MenuBuilder.BeginSection("Channels", LOCTEXT("Channels", "Channels"));
	{
	/*	for (int32 Index = 0; Index < ChannelsInfo.Num(); ++Index)
		{
			const FChannelData& Data = ChannelsInfo[Index];
			FString ChannelDisplayName = Data.Name;
			ChannelDisplayName.RemoveFromEnd(TEXT("Channel"), 7);

			MenuBuilder.AddMenuEntry(
				FText::FromString(ChannelDisplayName),
				FText::Format(LOCTEXT("ChannelDesc", "Enable/disable the {0} channel"), FText::FromString(ChannelDisplayName)),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleChannel_Execute, Index),
					FCanExecuteAction::CreateLambda([Value = !Data.bIsReadOnly]() { return Value; }),
					FIsActionChecked::CreateSP(this, &SDevC0DTStatusBarWidget::ToggleChannel_IsChecked, Index)),
				NAME_None,
				EUserInterfaceActionType::ToggleButton);
		}*/
	}
	MenuBuilder.EndSection();
}

void SDevC0DTStatusBarWidget::Traces_BuildMenu(FMenuBuilder& MenuBuilder)
{
	//CacheTraceStorePath();
	//PopulateRecentTracesList();

	//for (int32 Index = 0; Index < Traces.Num(); ++Index)
	//{
	//	MenuBuilder.AddMenuEntry(
	//		FUIAction(FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::OpenTrace, Index),
	//				  FCanExecuteAction()),
	//		SNew(SRecentTracesListEntry, Traces[Index], TraceStorePath, LiveSessionTracker),
	//		NAME_None,
	//		FText::FromString(Traces[Index]->FilePath),
	//		EUserInterfaceActionType::Button);
	//}
}

void SDevC0DTStatusBarWidget::InitCommandList()
{
	//FInsightsStatusBarWidgetCommands::Register();
	CommandList = MakeShared<FUICommandList>();
	//CommandList->MapAction(FInsightsStatusBarWidgetCommands::Get().Command_TraceScreenshot, FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::TraceScreenshot_Execute), FCanExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::TraceScreenshot_CanExecute));
	//CommandList->MapAction(FInsightsStatusBarWidgetCommands::Get().Command_TraceBookmark, FExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::TraceBookmark_Execute), FCanExecuteAction::CreateSP(this, &SDevC0DTStatusBarWidget::TraceBookmark_CanExecute));

}

FText SDevC0DTStatusBarWidget::GetTitleToolTipText() const
{
	FTextBuilder DescBuilder;
	DescBuilder.AppendLine(LOCTEXT("NotTracingText", "Not currently tracing."));
	/*const FString Dest = FTraceAuxiliary::GetTraceDestinationString();
	
	if (*Dest != 0)
	{
		DescBuilder.AppendLineFormat(LOCTEXT("TracingToText", "Tracing to: {0}"), FText::FromString(Dest));
	}
	else if (UE::Trace::IsTracing())
	{
		DescBuilder.AppendLine(LOCTEXT("TracingToUnknownText", "Tracing to unknown target (externally set)."));
	}
	else
	{
		DescBuilder.AppendLine(LOCTEXT("NotTracingText","Not currently tracing."));
	}*/
		
	return DescBuilder.ToText();
}

FSlateColor SDevC0DTStatusBarWidget::GetRecordingButtonColor() const
{
	//if (!UE::Trace::IsTracing())
	//{
	//	return FStyleColors::White;
	//}

	return FStyleColors::Error;
}

FSlateColor SDevC0DTStatusBarWidget::GetRecordingButtonOutlineColor() const
{
	/*if (!UE::Trace::IsTracing())
	{
		ConnectionStartTime = FSlateApplication::Get().GetCurrentTime();
		return FLinearColor::White.CopyWithNewOpacity(0.5f);
	}

	double ElapsedTime = FSlateApplication::Get().GetCurrentTime() - ConnectionStartTime;
	return FStyleColors::Error.GetColor(FWidgetStyle()).CopyWithNewOpacity(0.5f + 0.5f * FMath::MakePulsatingValue(ElapsedTime, 0.5f));*/
	return FStyleColors::Error;
}

FText SDevC0DTStatusBarWidget::GetRecordingButtonTooltipText() const
{
	//if (!UE::Trace::IsTracing())
	//{
	//	return LOCTEXT("StartTracing", "Start tracing. The trace destination is set from the menu.");
	//}

	return LOCTEXT("StopTracing", "Stop Tracing.");
}

void SDevC0DTStatusBarWidget::LogMessage(const FText& Text)
{
	//FMessageLog ReportMessageLog(LogListingName);
	//TSharedRef<FTokenizedMessage> TokenizedMessage = FTokenizedMessage::Create(EMessageSeverity::Error, Text);
	//ReportMessageLog.AddMessage(TokenizedMessage);
	//ReportMessageLog.Notify();
}

void SDevC0DTStatusBarWidget::ShowNotification(const FText& Text, const FText& SubText)
{
	//FNotificationInfo Info(Text);
	//Info.bFireAndForget = true;
	//Info.FadeOutDuration = 1.0f;
	//Info.ExpireDuration = 4.0f;

	//Info.SubText = SubText;

	//FSlateNotificationManager::Get().AddNotification(Info);
}

void SDevC0DTStatusBarWidget::LaunchUnrealInsights_OnClicked()
{
	//FUnrealInsightsLauncher::Get()->StartUnrealInsights(FUnrealInsightsLauncher::Get()->GetInsightsApplicationPath());
}

void SDevC0DTStatusBarWidget::OpenLiveSession_OnClicked()
{
	//OpenLiveSession(FTraceAuxiliary::GetTraceDestinationString());
}

void SDevC0DTStatusBarWidget::OpenLiveSession(const FString& InTraceDestination)
{
	//FUnrealInsightsLauncher::Get()->TryOpenTraceFromDestination(InTraceDestination);
}

void SDevC0DTStatusBarWidget::OpenProfilingDirectory_OnClicked()
{
	OpenProfilingDirectory();
}

void SDevC0DTStatusBarWidget::OpenProfilingDirectory()
{
	FString FullPath(FPaths::ConvertRelativePathToFull(FPaths::ProfilingDir()));

	if (!IFileManager::Get().DirectoryExists(*FullPath))
	{
		IFileManager::Get().MakeDirectory(*FullPath);
	}

	FPlatformProcess::ExploreFolder(*FullPath);
}

void SDevC0DTStatusBarWidget::OpenTraceStoreDirectory_OnClicked()
{
	OpenTraceStoreDirectory(ESelectLatestTraceCriteria::None);
}

void SDevC0DTStatusBarWidget::OpenTraceStoreDirectory(ESelectLatestTraceCriteria Criteria)
{
	CacheTraceStorePath();
	FString Path = TraceStorePath;

	if (Criteria != ESelectLatestTraceCriteria::None)
	{
		Path = GetLatestTraceFileFromFolder(TraceStorePath, Criteria);
	}

	FPlatformProcess::ExploreFolder(*Path);
}

void SDevC0DTStatusBarWidget::OpenLatestTraceFromFolder(const FString& InFolder, ESelectLatestTraceCriteria InCriteria)
{
	FString Path = GetLatestTraceFileFromFolder(InFolder, InCriteria);

	if (!Path.IsEmpty())
	{
		//FUnrealInsightsLauncher::Get()->TryOpenTraceFromDestination(Path);
	}
}

FString SDevC0DTStatusBarWidget::GetLatestTraceFileFromFolder(const FString& InFolder, ESelectLatestTraceCriteria InCriteria)
{
	FString Result;
	//FString MostRecentTraceName;
	//FDateTime LatestDateTime;

	//auto Visitor = [&MostRecentTraceName, &LatestDateTime, InCriteria](const TCHAR* Filename, const FFileStatData& StatData)
	//{
	//	if (FPaths::GetExtension(Filename) == TEXT("utrace"))
	//	{
	//		if (InCriteria == ESelectLatestTraceCriteria::ModifiedTime && LatestDateTime < StatData.ModificationTime)
	//		{
	//			LatestDateTime = StatData.ModificationTime;
	//			MostRecentTraceName = Filename;
	//		}

	//		if (InCriteria == ESelectLatestTraceCriteria::CreatedTime && LatestDateTime < StatData.CreationTime)
	//		{
	//			LatestDateTime = StatData.CreationTime;
	//			MostRecentTraceName = Filename;
	//		}
	//	}
	//	return true;
	//};

	//IFileManager::Get().IterateDirectoryStat(*InFolder, Visitor);

	//if (!MostRecentTraceName.IsEmpty())
	//{
	//	Result = FPaths::ConvertRelativePathToFull(MostRecentTraceName);
	//}

	return Result;
}

void SDevC0DTStatusBarWidget::SetTraceDestination_Execute(ETraceDestination InDestination)
{
	TraceDestination = InDestination;
}

bool SDevC0DTStatusBarWidget::SetTraceDestination_IsChecked(ETraceDestination InDestination)
{
	return InDestination == TraceDestination;
}

bool SDevC0DTStatusBarWidget::SetTraceDestination_CanExecute()
{
	//if (!UE::Trace::IsTracing())
	//{
	//	return true;
	//}

	return false;
}

void SDevC0DTStatusBarWidget::SaveSnapshot()
{
	//if (TraceDestination == ETraceDestination::File)
	//{
	//	const bool bResult = FTraceAuxiliary::WriteSnapshot(nullptr);
	//	if (bResult)
	//	{
	//		ShowNotification(LOCTEXT("SnapshotSavedHeading", "Insights Snapshot saved."), LOCTEXT("SnapshotSavedFileText", "A snapshot .utrace with the most recent events has been saved to your Saved/Profiling/ directory."));
	//		return;
	//	}
	//}
	//else
	//{
	//	const bool bResult = FTraceAuxiliary::SendSnapshot(nullptr);
	//	if (bResult)
	//	{
	//		ShowNotification(LOCTEXT("SnapshotSavedHeading", "Insights Snapshot saved."), LOCTEXT("SnapshotSavedServerText", "A snapshot .utrace with the most recent events has been saved to your trace server."));
	//		return;
	//	}
	//}
	//LogMessage(LOCTEXT("SnapshotSavedError", "The snapshot could not be saved."));
}

bool SDevC0DTStatusBarWidget::SaveSnapshot_CanExecute()
{
	return true;
}

FText SDevC0DTStatusBarWidget::GetTraceMenuItemText() const
{
	//if (UE::Trace::IsTracing())
	//{
	//	return LOCTEXT("StopTraceButtonText", "Stop Trace");
	//}

	return LOCTEXT("StartTraceButtonText", "Start Trace");
}

FText SDevC0DTStatusBarWidget::GetTraceMenuItemTooltipText() const
{
	//if (UE::Trace::IsTracing())
	//{
	//	return LOCTEXT("StopTraceButtonTooltip", "Stop tracing");
	//}

	return LOCTEXT("StartTraceButtonTooltip", "Start tracing to the selected trace destination.");
}

void SDevC0DTStatusBarWidget::ToggleTracing_OnClicked()
{
	//if (UE::Trace::IsTracing())
	//{
	//	bool bResult = FTraceAuxiliary::Stop();
	//	if (!bResult)
	//	{
	//		LogMessage(LOCTEXT("TraceStopFailedMsg", "There was no trace connection to stop."));
	//	}
	//}
	//else
	//{
	//	bool bResult = StartTracing();
	//	if (bResult)
	//	{
	//		FString TraceDestinationStr = FTraceAuxiliary::GetTraceDestinationString();
	//		if (TraceDestinationStr.IsEmpty())
	//		{
	//			TraceDestinationStr = TEXT("External Target");
	//		}

	//		ShowNotification(LOCTEXT("TraceMsg", "Trace Started"), FText::Format(LOCTEXT("TracingStartedText", "Trace is now active and saving to the following location (file or tracestore):\n{0}"), FText::FromString(TraceDestinationStr)));
	//	}
	//	else
	//	{
	//		LogMessage(LOCTEXT("TraceFailedToStartMsg", "Trace Failed to Start."));
	//	}
	//}
}

bool SDevC0DTStatusBarWidget::StartTracing()
{
	//if (TraceDestination == ETraceDestination::TraceStore)
	//{
	//	return FTraceAuxiliary::Start(FTraceAuxiliary::EConnectionType::Network, TEXT("localhost"), nullptr);
	//}
	//else if (TraceDestination == ETraceDestination::File)
	//{
	//	return FTraceAuxiliary::Start(FTraceAuxiliary::EConnectionType::File, nullptr, nullptr);
	//}

	return false;
}

EVisibility SDevC0DTStatusBarWidget::GetStartTraceIconVisibility() const
{
	//if (GetStopTraceIconVisibility() == EVisibility::Hidden)
	//{
	//	return EVisibility::Visible;
	//}

	return EVisibility::Hidden;
}

EVisibility SDevC0DTStatusBarWidget::GetStopTraceIconVisibility() const
{
	//if (bIsTraceRecordButtonHovered && UE::Trace::IsTracing())
	//{
	//	return EVisibility::Visible;
	//}

	return EVisibility::Hidden;
}

bool SDevC0DTStatusBarWidget::GetBooleanSettingValue(const TCHAR* InSettingName)
{
	bool Value = false;
	//GConfig->GetBool(SettingsCategory, InSettingName, Value, FEditorTraceUtilitiesModule::GetTraceUtilitiesIni());
	return Value;
}

void SDevC0DTStatusBarWidget::ToggleBooleanSettingValue(const TCHAR* InSettingName)
{
	bool Value = false;
	//GConfig->GetBool(SettingsCategory, InSettingName, Value, FEditorTraceUtilitiesModule::GetTraceUtilitiesIni());
	//GConfig->SetBool(SettingsCategory, InSettingName, !Value, FEditorTraceUtilitiesModule::GetTraceUtilitiesIni());
}

void SDevC0DTStatusBarWidget::OnTraceStarted(FTraceAuxiliary::EConnectionType InTraceType, const FString& InTraceDestination)
{
	/*if (InTraceType == FTraceAuxiliary::EConnectionType::Network && GetBooleanSettingValue(OpenLiveSessionOnTraceStartSettingName))
	{
		TGraphTask<FOpenLiveSessionTask>::CreateTask().ConstructAndDispatchWhenReady();
	}*/
}

void SDevC0DTStatusBarWidget::OnTraceStopped(FTraceAuxiliary::EConnectionType InTraceType, const FString& InTraceDestination)
{
	//if (GetBooleanSettingValue(OpenInsightsAfterTraceSettingName))
	//{
	//	OpenLiveSession(InTraceDestination);
	//}
	//if (GetBooleanSettingValue(ShowInExplorerAfterTraceSettingName))
	//{
	//	if (InTraceType == FTraceAuxiliary::EConnectionType::Network)
	//	{
	//		OpenTraceStoreDirectory(ESelectLatestTraceCriteria::ModifiedTime);
	//	}
	//	else if (InTraceType == FTraceAuxiliary::EConnectionType::File)
	//	{
	//		FPlatformProcess::ExploreFolder(*InTraceDestination);
	//	}
	//}
}

void SDevC0DTStatusBarWidget::OnSnapshotSaved(FTraceAuxiliary::EConnectionType InTraceType, const FString& InTraceDestination)
{
	/*if (GetBooleanSettingValue(OpenInsightsAfterTraceSettingName))
	{
		if (InTraceType == FTraceAuxiliary::EConnectionType::Network)
		{
			CacheTraceStorePath();
			OpenLatestTraceFromFolder(TraceStorePath, ESelectLatestTraceCriteria::CreatedTime);
		}
		else if (InTraceType == FTraceAuxiliary::EConnectionType::File)
		{
			FUnrealInsightsLauncher::Get()->TryOpenTraceFromDestination(InTraceDestination);
		}
	}
	if (GetBooleanSettingValue(ShowInExplorerAfterTraceSettingName))
	{
		if (InTraceType == FTraceAuxiliary::EConnectionType::Network)
		{
			OpenTraceStoreDirectory(ESelectLatestTraceCriteria::CreatedTime);
		}
		else if (InTraceType == FTraceAuxiliary::EConnectionType::File)
		{
			FPlatformProcess::ExploreFolder(*InTraceDestination);
		}
	}*/
}

void SDevC0DTStatusBarWidget::CacheTraceStorePath()
{
	/*if (TraceStorePath.IsEmpty())
	{
		UE::Trace::FStoreClient* StoreClient = UE::Trace::FStoreClient::Connect(TEXT("localhost"));

		if (!StoreClient)
		{
			LogMessage(LOCTEXT("FailedConnectionToStoreMsg", "Failed to connect to the store client."));
			return;
		}

		const UE::Trace::FStoreClient::FStatus* Status = StoreClient->GetStatus();
		if (!Status)
		{
			LogMessage(LOCTEXT("FailedToGetStoreStatusMsg", "Failed to get the status of the store client."));
			return;
		}

		TraceStorePath = FString(Status->GetStoreDir());
	}*/
}

void SDevC0DTStatusBarWidget::CreateChannelsInfo()
{
#if UE_TRACE_ENABLED
	//ChannelsInfo.Empty();

	//UE::Trace::EnumerateChannels([](const UE::Trace::FChannelInfo& Info, void* User)
	//{
	//	TArray<FChannelData>* Channels = (TArray<FChannelData>*) User;

	//	FChannelData NewChannelInfo;
	//	NewChannelInfo.Name = Info.Name;
	//	NewChannelInfo.bIsEnabled = Info.bIsEnabled;
	//	NewChannelInfo.bIsReadOnly = Info.bIsReadOnly;

	//	Channels->Add(NewChannelInfo);
	//	return true;
	//}, &ChannelsInfo);

	//Algo::SortBy(ChannelsInfo, [](const FChannelData& Entry) { return Entry.Name; }, [](const FString& Rhs, const FString& Lhs)
	//	{
	//		return Rhs.Compare(Lhs) < 0;
	//	});
#endif
}

void SDevC0DTStatusBarWidget::UpdateChannelsInfo()
{
#if UE_TRACE_ENABLED
	//UE::Trace::EnumerateChannels([](const UE::Trace::FChannelInfo& Info, void* User)
	//{
	//	TArray<FChannelData>* Channels = (TArray<FChannelData>*) User;

	//	FString Name = Info.Name;
	//	int32 Index = Algo::BinarySearchBy(*Channels, Name, [](const FChannelData& Entry) { return Entry.Name; }, [](const FString& Rhs, const FString& Lhs)
	//		{
	//			return Rhs.Compare(Lhs) < 0;
	//		});

	//	if (Index != INDEX_NONE)
	//	{
	//		FChannelData& Data = (*Channels)[Index];
	//		Data.Name = Info.Name;
	//		Data.bIsEnabled = Info.bIsEnabled;
	//		Data.bIsReadOnly = Info.bIsReadOnly;
	//	}

	//	return true;
	//}, &ChannelsInfo);

	//bShouldUpdateChannels = false;
#endif
}

void SDevC0DTStatusBarWidget::ToggleChannel_Execute(int32 Index)
{
	//if (Index < ChannelsInfo.Num())
	//{
	//	UE::Trace::ToggleChannel(*ChannelsInfo[Index].Name, !ChannelsInfo[Index].bIsEnabled);
	//}
}

bool SDevC0DTStatusBarWidget::ToggleChannel_IsChecked(int32 Index)
{
	//if (bShouldUpdateChannels)
	//{
	//	UpdateChannelsInfo();
	//}

	//if (Index < ChannelsInfo.Num())
	//{
	//	return ChannelsInfo[Index].bIsEnabled;
	//}

	return false;
}


bool SDevC0DTStatusBarWidget::TraceScreenshot_CanExecute()
{
	//return SHOULD_TRACE_SCREENSHOT();

	return false;
}

void SDevC0DTStatusBarWidget::TraceScreenshot_Execute()
{
	//FTraceScreenshot::RequestScreenshot(TEXT(""), false);
}

bool SDevC0DTStatusBarWidget::TraceBookmark_CanExecute()
{
	//return SHOULD_TRACE_BOOKMARK();
	return false;
}

void SDevC0DTStatusBarWidget::TraceBookmark_Execute()
{
	//FString Bookmark = FDateTime::Now().ToString(TEXT("Bookmark_%Y%m%d_%H%M%S"));
	//TRACE_BOOKMARK(*Bookmark);
}

void SDevC0DTStatusBarWidget::PopulateRecentTracesList()
{
	//Traces.Empty();
	//bool bIsFromTraceStore = true;
	//auto Visitor = [this, &bIsFromTraceStore](const TCHAR* Filename, const FFileStatData& StatData)
	//{
	//	if (FPaths::GetExtension(Filename) == TEXT("utrace"))
	//	{
	//		TSharedPtr<FTraceFileInfo> TraceInfo = MakeShared<FTraceFileInfo>();
	//		TraceInfo->FilePath = Filename;
	//		TraceInfo->ModifiedTime = StatData.ModificationTime;
	//		TraceInfo->bIsFromTraceStore = bIsFromTraceStore;

	//		Traces.Add(TraceInfo);
	//	}

	//	return true;
	//};

	//IFileManager::Get().IterateDirectoryStat(*TraceStorePath, Visitor);

	//bIsFromTraceStore = false;
	//IFileManager::Get().IterateDirectoryStat(*FPaths::ProfilingDir(), Visitor);

	//Algo::SortBy(Traces, [](TSharedPtr<FTraceFileInfo> TraceInfo) { return *TraceInfo; });

	//constexpr int32 MaxRecentTraces = 15;
	//if (Traces.Num() > MaxRecentTraces)
	//{
	//	Traces.RemoveAt(MaxRecentTraces - 1, Traces.Num() - MaxRecentTraces);
	//}
}

void SDevC0DTStatusBarWidget::OpenTrace(int32 Index)
{
	//if (Index < Traces.Num())
	//{
	//	if (FTraceAuxiliary::GetConnectionType() == FTraceAuxiliary::EConnectionType::Network)
	//	{
	//		if (LiveSessionTracker->HasData())
	//		{
	//			const FLiveSessionsMap& Sessions = LiveSessionTracker->GetLiveSessions();
	//			FString FileName = FPaths::GetBaseFilename(Traces[Index]->FilePath);
	//			const uint32* TraceId = Sessions.Find(FileName);
	//			
	//			if (TraceId)
	//			{
	//				FUnrealInsightsLauncher::Get()->OpenRemoteTrace(TEXT("localhost"), uint16(LiveSessionTracker->GetStorePort()), *TraceId);
	//				return;
	//			}
	//		}
	//	}

	//	FUnrealInsightsLauncher::Get()->TryOpenTraceFromDestination(Traces[Index]->FilePath);
	//}
}

#undef LOCTEXT_NAMESPACE
