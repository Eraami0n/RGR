#pragma once
#include <windows.h>
#include <commctrl.h>
#include "SportsClubManager.h"

class MainForm {
private:
    HWND hwndMain;
    HWND hwndTab;
    
    // Athletes tab
    HWND hwndAthletesTab;
    HWND hwndAthletesList;
    HWND hwndAddAthleteBtn;
    HWND hwndRemoveAthleteBtn;
    HWND hwndAthleteNameEdit;
    HWND hwndAthleteCategoryEdit;
    HWND hwndAthletePhoneEdit;
    HWND hwndAthleteFeeEdit;
    
    // Trainings tab
    HWND hwndTrainingsTab;
    HWND hwndTrainingsList;
    HWND hwndAddTrainingBtn;
    HWND hwndRemoveTrainingBtn;
    HWND hwndTrainingDateEdit;
    HWND hwndTrainingTimeEdit;
    HWND hwndTrainingTrainerEdit;
    HWND hwndTrainingLocationEdit;
    
    // Payments tab
    HWND hwndPaymentsTab;
    HWND hwndPaymentsList;
    HWND hwndAddPaymentBtn;
    HWND hwndMarkPaidBtn;
    HWND hwndPaymentAmountEdit;
    HWND hwndPaymentDateEdit;
    
    // Stats tab
    HWND hwndStatsTab;
    HWND hwndStatsText;
    HWND hwndRefreshStatsBtn;

    // Manager instance
    SportsClubManager* manager;

    // Dialog proc
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    // UI creation
    void CreateUI(HWND hwnd);
    void CreateAthletesTab();
    void CreateTrainingsTab();
    void CreatePaymentsTab();
    void CreateStatsTab();

    // Data refresh
    void RefreshAthletesList();
    void RefreshTrainingsList();
    void RefreshPaymentsList();
    void RefreshStats();

    // Event handlers
    void OnAddAthlete();
    void OnRemoveAthlete();
    void OnAddTraining();
    void OnRemoveTraining();
    void OnAddPayment();
    void OnMarkPaid();
    void OnRefreshStats();

    // Helper functions
    std::string GetEditText(HWND hwndEdit);
    void SetEditText(HWND hwndEdit, const std::string& text);
    void ClearEditFields();

public:
    MainForm(SportsClubManager* mgr);
    ~MainForm();

    int Run(HINSTANCE hInstance);
};