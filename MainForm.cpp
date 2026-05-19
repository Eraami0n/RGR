#include "MainForm.h"
#include <sstream>
#include <iomanip>

MainForm::MainForm(SportsClubManager* mgr) 
    : hwndMain(nullptr), hwndTab(nullptr), manager(mgr) {}

MainForm::~MainForm() {}

LRESULT CALLBACK MainForm::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_CREATE) {
        MainForm* pThis = reinterpret_cast<MainForm*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return 0;
    }

    MainForm* pThis = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (pThis) {
        return pThis->HandleMessage(msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT MainForm::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwndMain);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwndMain, msg, wParam, lParam);
    }
    return 0;
}

void MainForm::CreateUI(HWND hwnd) {
    hwndMain = hwnd;

    hwndTab = CreateWindowEx(
        0, WC_TABCONTROL, L"",
        WS_CHILD | WS_VISIBLE,
        10, 10, 780, 540,
        hwnd, (HMENU)1000, GetModuleHandle(nullptr), nullptr
    );

    TCITEM tie;
    tie.mask = TCIF_TEXT;

    tie.pszText = (LPWSTR)L"Sportsmen";
    TabCtrl_InsertItem(hwndTab, 0, &tie);

    tie.pszText = (LPWSTR)L"Trainings";
    TabCtrl_InsertItem(hwndTab, 1, &tie);

    tie.pszText = (LPWSTR)L"Payments";
    TabCtrl_InsertItem(hwndTab, 2, &tie);

    tie.pszText = (LPWSTR)L"Statistics";
    TabCtrl_InsertItem(hwndTab, 3, &tie);

    CreateAthletesTab();
    CreateTrainingsTab();
    CreatePaymentsTab();
    CreateStatsTab();
}

void MainForm::CreateAthletesTab() {
    hwndAthletesList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"ID";
    ListView_InsertColumn(hwndAthletesList, 0, &lvc);

    lvc.pszText = (LPWSTR)L"Name";
    ListView_InsertColumn(hwndAthletesList, 1, &lvc);

    lvc.pszText = (LPWSTR)L"Category";
    ListView_InsertColumn(hwndAthletesList, 2, &lvc);

    RefreshAthletesList();
}

void MainForm::CreateTrainingsTab() {
    hwndTrainingsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = 100;

    lvc.pszText = (LPWSTR)L"ID";
    ListView_InsertColumn(hwndTrainingsList, 0, &lvc);

    lvc.pszText = (LPWSTR)L"Date";
    ListView_InsertColumn(hwndTrainingsList, 1, &lvc);

    RefreshTrainingsList();
}

void MainForm::CreatePaymentsTab() {
    hwndPaymentsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = 100;

    lvc.pszText = (LPWSTR)L"Athlete";
    ListView_InsertColumn(hwndPaymentsList, 0, &lvc);

    lvc.pszText = (LPWSTR)L"Amount";
    ListView_InsertColumn(hwndPaymentsList, 1, &lvc);

    RefreshPaymentsList();
}

void MainForm::CreateStatsTab() {
    hwndStatsText = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"STATIC", L"",
        WS_CHILD | WS_VISIBLE,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );
    RefreshStats();
}

void MainForm::RefreshAthletesList() {
    ListView_DeleteAllItems(hwndAthletesList);
    const auto& athletes = manager->getAthletes();
    
    for (size_t i = 0; i < athletes.size(); i++) {
        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        
        std::string idStr = athletes[i].id;
        std::wstring wid(idStr.begin(), idStr.end());
        lvi.pszText = (LPWSTR)wid.c_str();
        ListView_InsertItem(hwndAthletesList, &lvi);
        
        std::string nameStr = athletes[i].name;
        std::wstring wname(nameStr.begin(), nameStr.end());
        ListView_SetItemText(hwndAthletesList, i, 1, (LPWSTR)wname.c_str());
    }
}

void MainForm::RefreshTrainingsList() {
    ListView_DeleteAllItems(hwndTrainingsList);
    const auto& trainings = manager->getTrainings();
    
    for (size_t i = 0; i < trainings.size(); i++) {
        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        
        std::string idStr = trainings[i].id;
        std::wstring wid(idStr.begin(), idStr.end());
        lvi.pszText = (LPWSTR)wid.c_str();
        ListView_InsertItem(hwndTrainingsList, &lvi);
    }
}

void MainForm::RefreshPaymentsList() {
    ListView_DeleteAllItems(hwndPaymentsList);
    const auto& payments = manager->getPayments();
    
    for (size_t i = 0; i < payments.size(); i++) {
        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        
        std::string nameStr = payments[i].athleteName;
        std::wstring wname(nameStr.begin(), nameStr.end());
        lvi.pszText = (LPWSTR)wname.c_str();
        ListView_InsertItem(hwndPaymentsList, &lvi);
    }
}

void MainForm::RefreshStats() {
    std::ostringstream oss;
    oss << "Total Athletes: " << manager->getTotalAthletes() << "\n";
    oss << "Total Revenue: " << manager->getTotalRevenue() << "\n";
    oss << "Unpaid Payments: " << manager->getUnpaidPayments() << "\n";
    
    std::string statsStr = oss.str();
    std::wstring wstats(statsStr.begin(), statsStr.end());
    SetWindowText(hwndStatsText, wstats.c_str());
}

int MainForm::Run(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"SportsClubManagerClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Sports Club Manager",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, this
    );

    if (!hwnd) return 0;

    CreateUI(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
