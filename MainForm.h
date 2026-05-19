#pragma once
#include <windows.h>
#include <commctrl.h>
#include "SportsClubManager.h"

class MainForm {
private:
    HWND hwndMain;
    HWND hwndTab;
    HWND hwndAthletesTab;
    HWND hwndTrainingsTab;
    HWND hwndPaymentsTab;
    HWND hwndStatsTab;
    
    HWND hwndAthletesList;
    HWND hwndTrainingsList;
    HWND hwndPaymentsList;
    HWND hwndStatsText;

    SportsClubManager manager;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    void CreateUI(HWND hwnd);
    void CreateAthletesTab();
    void CreateTrainingsTab();
    void CreatePaymentsTab();
    void CreateStatsTab();

    void RefreshAthletesList();
    void RefreshTrainingsList();
    void RefreshPaymentsList();
    void RefreshStats();

public:
    MainForm();
    ~MainForm();

    int Run(HINSTANCE hInstance);
};