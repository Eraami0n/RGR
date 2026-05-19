#include "MainForm.h"
#include <sstream>
#include <iomanip>

MainForm::MainForm() : hwndMain(nullptr), hwndTab(nullptr) {}

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
    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == 1 && ((LPNMHDR)lParam)->code == TCN_SELCHANGE) {
            int nTab = TabCtrl_GetCurSel(hwndTab);
            // Handle tab change
        }
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            // Handle button clicks
            if (LOWORD(wParam) == 101) {
                // Add athlete button
                std::string name = "New Athlete";
                manager.addAthlete("1", name, "Beginner", "555-0000", 100.0);
                RefreshAthletesList();
            }
        }
        break;
    default:
        return DefWindowProc(hwndMain, msg, wParam, lParam);
    }
    return 0;
}

void MainForm::CreateUI(HWND hwnd) {
    hwndMain = hwnd;

    // Create Tab Control
    hwndTab = CreateWindowEx(
        0, WC_TABCONTROL, L"",
        WS_CHILD | WS_VISIBLE,
        10, 10, 780, 540,
        hwnd, (HMENU)1, GetModuleHandle(nullptr), nullptr
    );

    // Create tabs
    TCITEM tie;
    tie.mask = TCIF_TEXT;

    tie.pszText = (LPWSTR)L"Спортсмены";
    TabCtrl_InsertItem(hwndTab, 0, &tie);

    tie.pszText = (LPWSTR)L"Тренировки";
    TabCtrl_InsertItem(hwndTab, 1, &tie);

    tie.pszText = (LPWSTR)L"Платежи";
    TabCtrl_InsertItem(hwndTab, 2, &tie);

    tie.pszText = (LPWSTR)L"Статистика";
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

    lvc.pszText = (LPWSTR)L"ПИБ";
    ListView_InsertColumn(hwndAthletesList, 1, &lvc);

    lvc.pszText = (LPWSTR)L"Категория";
    ListView_InsertColumn(hwndAthletesList, 2, &lvc);

    lvc.pszText = (LPWSTR)L"Телефон";
    ListView_InsertColumn(hwndAthletesList, 3, &lvc);

    lvc.pszText = (LPWSTR)L"Сумма";
    ListView_InsertColumn(hwndAthletesList, 4, &lvc);
}

void MainForm::CreateTrainingsTab() {
    hwndTrainingsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );
}

void MainForm::CreatePaymentsTab() {
    hwndPaymentsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT,
        20, 50, 750, 400,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );
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
    const auto& athletes = manager.getAthletes();
    
    for (size_t i = 0; i < athletes.size(); i++) {
        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = (LPWSTR)std::wstring(athletes[i].id.begin(), athletes[i].id.end()).c_str();
        ListView_InsertItem(hwndAthletesList, &lvi);
    }
}

void MainForm::RefreshTrainingsList() {
    ListView_DeleteAllItems(hwndTrainingsList);
}

void MainForm::RefreshPaymentsList() {
    ListView_DeleteAllItems(hwndPaymentsList);
}

void MainForm::RefreshStats() {
    std::wostringstream oss;
    oss << L"Всего спортсменов: " << manager.getTotalAthletes() << L"\n";
    oss << L"Общий доход: " << std::fixed << std::setprecision(2) << manager.getTotalRevenue() << L"\n";
    oss << L"Неоплаченные платежи: " << manager.getUnpaidPayments() << L"\n";
    
    SetWindowText(hwndStatsText, oss.str().c_str());
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
        0, CLASS_NAME, L"Менеджер спортивного клуба",
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