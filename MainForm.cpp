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
    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == 1000 && ((LPNMHDR)lParam)->code == TCN_SELCHANGE) {
            int nTab = TabCtrl_GetCurSel(hwndTab);
            // Handle tab change - could add logic here
        }
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
            case 101:  // Add athlete
                OnAddAthlete();
                break;
            case 102:  // Remove athlete
                OnRemoveAthlete();
                break;
            case 201:  // Add training
                OnAddTraining();
                break;
            case 202:  // Remove training
                OnRemoveTraining();
                break;
            case 301:  // Add payment
                OnAddPayment();
                break;
            case 302:  // Mark paid
                OnMarkPaid();
                break;
            case 401:  // Refresh stats
                OnRefreshStats();
                break;
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
        hwnd, (HMENU)1000, GetModuleHandle(nullptr), nullptr
    );

    // Create tabs
    TCITEM tie;
    tie.mask = TCIF_TEXT;

    tie.pszText = (LPWSTR)L"Спортсмени";
    TabCtrl_InsertItem(hwndTab, 0, &tie);

    tie.pszText = (LPWSTR)L"Тренування";
    TabCtrl_InsertItem(hwndTab, 1, &tie);

    tie.pszText = (LPWSTR)L"Платежі";
    TabCtrl_InsertItem(hwndTab, 2, &tie);

    tie.pszText = (LPWSTR)L"Статистика";
    TabCtrl_InsertItem(hwndTab, 3, &tie);

    CreateAthletesTab();
    CreateTrainingsTab();
    CreatePaymentsTab();
    CreateStatsTab();
}

void MainForm::CreateAthletesTab() {
    // Athletes list
    hwndAthletesList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        20, 50, 700, 300,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 80;
    lvc.pszText = (LPWSTR)L"ID";
    ListView_InsertColumn(hwndAthletesList, 0, &lvc);

    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"ПІБ";
    ListView_InsertColumn(hwndAthletesList, 1, &lvc);

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"Категорія";
    ListView_InsertColumn(hwndAthletesList, 2, &lvc);

    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"Телефон";
    ListView_InsertColumn(hwndAthletesList, 3, &lvc);

    lvc.cx = 90;
    lvc.pszText = (LPWSTR)L"Внесок";
    ListView_InsertColumn(hwndAthletesList, 4, &lvc);

    // Input fields
    CreateWindowEx(0, L"STATIC", L"ПІБ:", WS_CHILD | WS_VISIBLE, 20, 360, 50, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndAthleteNameEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 80, 360, 150, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Категорія:", WS_CHILD | WS_VISIBLE, 240, 360, 80, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndAthleteCategoryEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 330, 360, 150, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Телефон:", WS_CHILD | WS_VISIBLE, 490, 360, 70, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndAthletePhoneEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 570, 360, 150, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Внесок:", WS_CHILD | WS_VISIBLE, 20, 390, 50, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndAthleteFeeEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, 80, 390, 100, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    // Buttons
    hwndAddAthleteBtn = CreateWindowEx(0, L"BUTTON", L"Додати спортсмена", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 390, 150, 25, hwndMain, (HMENU)101, GetModuleHandle(nullptr), nullptr);
    hwndRemoveAthleteBtn = CreateWindowEx(0, L"BUTTON", L"Видалити", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 370, 390, 100, 25, hwndMain, (HMENU)102, GetModuleHandle(nullptr), nullptr);

    RefreshAthletesList();
}

void MainForm::CreateTrainingsTab() {
    // Trainings list
    hwndTrainingsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
        20, 50, 700, 300,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 80;
    lvc.pszText = (LPWSTR)L"ID";
    ListView_InsertColumn(hwndTrainingsList, 0, &lvc);

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"Дата";
    ListView_InsertColumn(hwndTrainingsList, 1, &lvc);

    lvc.cx = 80;
    lvc.pszText = (LPWSTR)L"Час";
    ListView_InsertColumn(hwndTrainingsList, 2, &lvc);

    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"Тренер";
    ListView_InsertColumn(hwndTrainingsList, 3, &lvc);

    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"Місцезнаходження";
    ListView_InsertColumn(hwndTrainingsList, 4, &lvc);

    lvc.cx = 80;
    lvc.pszText = (LPWSTR)L"Учасники";
    ListView_InsertColumn(hwndTrainingsList, 5, &lvc);

    // Input fields
    CreateWindowEx(0, L"STATIC", L"Дата:", WS_CHILD, 20, 360, 50, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndTrainingDateEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 80, 360, 120, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Час:", WS_CHILD, 210, 360, 40, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndTrainingTimeEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 260, 360, 100, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Тренер:", WS_CHILD, 370, 360, 60, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndTrainingTrainerEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 440, 360, 130, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Місцезнаходження:", WS_CHILD, 20, 390, 130, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndTrainingLocationEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 160, 390, 200, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    // Buttons
    hwndAddTrainingBtn = CreateWindowEx(0, L"BUTTON", L"Додати тренування", WS_CHILD | BS_PUSHBUTTON, 380, 390, 150, 25, hwndMain, (HMENU)201, GetModuleHandle(nullptr), nullptr);
    hwndRemoveTrainingBtn = CreateWindowEx(0, L"BUTTON", L"Видалити", WS_CHILD | BS_PUSHBUTTON, 550, 390, 100, 25, hwndMain, (HMENU)202, GetModuleHandle(nullptr), nullptr);

    RefreshTrainingsList();
}

void MainForm::CreatePaymentsTab() {
    // Payments list
    hwndPaymentsList = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
        20, 50, 700, 300,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"ID спортсмена";
    ListView_InsertColumn(hwndPaymentsList, 0, &lvc);

    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"Ім'я";
    ListView_InsertColumn(hwndPaymentsList, 1, &lvc);

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"Сума";
    ListView_InsertColumn(hwndPaymentsList, 2, &lvc);

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"Дата";
    ListView_InsertColumn(hwndPaymentsList, 3, &lvc);

    lvc.cx = 100;
    lvc.pszText = (LPWSTR)L"Статус";
    ListView_InsertColumn(hwndPaymentsList, 4, &lvc);

    // Input fields
    CreateWindowEx(0, L"STATIC", L"Сума:", WS_CHILD, 20, 360, 50, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndPaymentAmountEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | ES_AUTOHSCROLL | ES_NUMBER, 80, 360, 100, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"STATIC", L"Дата:", WS_CHILD, 190, 360, 50, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);
    hwndPaymentDateEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | ES_AUTOHSCROLL, 250, 360, 120, 20, hwndMain, nullptr, GetModuleHandle(nullptr), nullptr);

    // Buttons
    hwndAddPaymentBtn = CreateWindowEx(0, L"BUTTON", L"Додати платіж", WS_CHILD | BS_PUSHBUTTON, 390, 360, 130, 25, hwndMain, (HMENU)301, GetModuleHandle(nullptr), nullptr);
    hwndMarkPaidBtn = CreateWindowEx(0, L"BUTTON", L"Позначити оплачено", WS_CHILD | BS_PUSHBUTTON, 540, 360, 150, 25, hwndMain, (HMENU)302, GetModuleHandle(nullptr), nullptr);

    RefreshPaymentsList();
}

void MainForm::CreateStatsTab() {
    hwndStatsText = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"STATIC", L"",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        20, 50, 700, 350,
        hwndMain, nullptr, GetModuleHandle(nullptr), nullptr
    );

    hwndRefreshStatsBtn = CreateWindowEx(
        0, L"BUTTON", L"Оновити статистику",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        20, 410, 150, 25,
        hwndMain, (HMENU)401, GetModuleHandle(nullptr), nullptr
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
        
        // ID
        lvi.iSubItem = 0;
        std::wstring wid(athletes[i].id.begin(), athletes[i].id.end());
        lvi.pszText = (LPWSTR)wid.c_str();
        ListView_InsertItem(hwndAthletesList, &lvi);
        
        // Name
        lvi.iSubItem = 1;
        std::wstring wname(athletes[i].name.begin(), athletes[i].name.end());
        ListView_SetItemText(hwndAthletesList, i, 1, (LPWSTR)wname.c_str());
        
        // Category
        lvi.iSubItem = 2;
        std::wstring wcat(athletes[i].category.begin(), athletes[i].category.end());
        ListView_SetItemText(hwndAthletesList, i, 2, (LPWSTR)wcat.c_str());
        
        // Phone
        lvi.iSubItem = 3;
        std::wstring wphone(athletes[i].phone.begin(), athletes[i].phone.end());
        ListView_SetItemText(hwndAthletesList, i, 3, (LPWSTR)wphone.c_str());
        
        // Fee
        lvi.iSubItem = 4;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << athletes[i].monthlyFee << " грн";
        std::wstring wfee(oss.str().begin(), oss.str().end());
        ListView_SetItemText(hwndAthletesList, i, 4, (LPWSTR)wfee.c_str());
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
        
        std::wstring wid(trainings[i].id.begin(), trainings[i].id.end());
        lvi.pszText = (LPWSTR)wid.c_str();
        ListView_InsertItem(hwndTrainingsList, &lvi);
        
        std::wstring wdate(trainings[i].date.begin(), trainings[i].date.end());
        ListView_SetItemText(hwndTrainingsList, i, 1, (LPWSTR)wdate.c_str());
        
        std::wstring wtime(trainings[i].time.begin(), trainings[i].time.end());
        ListView_SetItemText(hwndTrainingsList, i, 2, (LPWSTR)wtime.c_str());
        
        std::wstring wtrainer(trainings[i].trainer.begin(), trainings[i].trainer.end());
        ListView_SetItemText(hwndTrainingsList, i, 3, (LPWSTR)wtrainer.c_str());
        
        std::wstring wloc(trainings[i].location.begin(), trainings[i].location.end());
        ListView_SetItemText(hwndTrainingsList, i, 4, (LPWSTR)wloc.c_str());
        
        std::wstring wpart = std::to_wstring(trainings[i].participants);
        ListView_SetItemText(hwndTrainingsList, i, 5, (LPWSTR)wpart.c_str());
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
        
        std::wstring waid(payments[i].athleteId.begin(), payments[i].athleteId.end());
        lvi.pszText = (LPWSTR)waid.c_str();
        ListView_InsertItem(hwndPaymentsList, &lvi);
        
        std::wstring wname(payments[i].athleteName.begin(), payments[i].athleteName.end());
        ListView_SetItemText(hwndPaymentsList, i, 1, (LPWSTR)wname.c_str());
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << payments[i].amount;
        std::wstring wamt(oss.str().begin(), oss.str().end());
        ListView_SetItemText(hwndPaymentsList, i, 2, (LPWSTR)wamt.c_str());
        
        std::wstring wdate(payments[i].paymentDate.begin(), payments[i].paymentDate.end());
        ListView_SetItemText(hwndPaymentsList, i, 3, (LPWSTR)wdate.c_str());
        
        std::wstring wstatus = payments[i].isPaid ? L"Оплачено" : L"Не оплачено";
        ListView_SetItemText(hwndPaymentsList, i, 4, (LPWSTR)wstatus.c_str());
    }
}

void MainForm::RefreshStats() {
    std::wstring stats = std::wstring(manager->getStatisticsString().begin(), 
                                      manager->getStatisticsString().end());
    SetWindowText(hwndStatsText, stats.c_str());
}

void MainForm::OnAddAthlete() {
    std::string name = GetEditText(hwndAthleteNameEdit);
    std::string category = GetEditText(hwndAthleteCategoryEdit);
    std::string phone = GetEditText(hwndAthletePhoneEdit);
    std::string feeStr = GetEditText(hwndAthleteFeeEdit);
    
    if (!name.empty() && !category.empty()) {
        double fee = std::stod(feeStr.empty() ? "0" : feeStr);
        std::string id = std::to_string(manager->getAthletes().size() + 1);
        manager->addAthlete(id, name, category, phone, fee);
        RefreshAthletesList();
        ClearEditFields();
    }
}

void MainForm::OnRemoveAthlete() {
    int selected = ListView_GetNextItem(hwndAthletesList, -1, LVNI_SELECTED);
    if (selected >= 0) {
        const auto& athletes = manager->getAthletes();
        if (selected < (int)athletes.size()) {
            manager->removeAthlete(athletes[selected].id);
            RefreshAthletesList();
        }
    }
}

void MainForm::OnAddTraining() {
    std::string date = GetEditText(hwndTrainingDateEdit);
    std::string time = GetEditText(hwndTrainingTimeEdit);
    std::string trainer = GetEditText(hwndTrainingTrainerEdit);
    std::string location = GetEditText(hwndTrainingLocationEdit);
    
    if (!date.empty() && !trainer.empty()) {
        std::string id = std::to_string(manager->getTrainings().size() + 1);
        manager->addTraining(id, date, time, trainer, location, 0);
        RefreshTrainingsList();
        ClearEditFields();
    }
}

void MainForm::OnRemoveTraining() {
    int selected = ListView_GetNextItem(hwndTrainingsList, -1, LVNI_SELECTED);
    if (selected >= 0) {
        const auto& trainings = manager->getTrainings();
        if (selected < (int)trainings.size()) {
            manager->removeTraining(trainings[selected].id);
            RefreshTrainingsList();
        }
    }
}

void MainForm::OnAddPayment() {
    int selected = ListView_GetNextItem(hwndAthletesList, -1, LVNI_SELECTED);
    if (selected >= 0) {
        const auto& athletes = manager->getAthletes();
        if (selected < (int)athletes.size()) {
            std::string amountStr = GetEditText(hwndPaymentAmountEdit);
            std::string date = GetEditText(hwndPaymentDateEdit);
            double amount = std::stod(amountStr.empty() ? "0" : amountStr);
            manager->addPayment(athletes[selected].id, athletes[selected].name, amount, date, false);
            RefreshPaymentsList();
            ClearEditFields();
        }
    }
}

void MainForm::OnMarkPaid() {
    int selected = ListView_GetNextItem(hwndPaymentsList, -1, LVNI_SELECTED);
    if (selected >= 0) {
        const auto& payments = manager->getPayments();
        if (selected < (int)payments.size()) {
            manager->updatePaymentStatus(payments[selected].athleteId, true);
            RefreshPaymentsList();
            RefreshStats();
        }
    }
}

void MainForm::OnRefreshStats() {
    RefreshStats();
}

std::string MainForm::GetEditText(HWND hwndEdit) {
    wchar_t buffer[256];
    GetWindowText(hwndEdit, buffer, 256);
    std::string result(buffer, buffer + wcslen(buffer));
    return result;
}

void MainForm::SetEditText(HWND hwndEdit, const std::string& text) {
    std::wstring wtext(text.begin(), text.end());
    SetWindowText(hwndEdit, wtext.c_str());
}

void MainForm::ClearEditFields() {
    SetWindowText(hwndAthleteNameEdit, L"");
    SetWindowText(hwndAthleteCategoryEdit, L"");
    SetWindowText(hwndAthletePhoneEdit, L"");
    SetWindowText(hwndAthleteFeeEdit, L"0");
    SetWindowText(hwndTrainingDateEdit, L"");
    SetWindowText(hwndTrainingTimeEdit, L"");
    SetWindowText(hwndTrainingTrainerEdit, L"");
    SetWindowText(hwndTrainingLocationEdit, L"");
    SetWindowText(hwndPaymentAmountEdit, L"0");
    SetWindowText(hwndPaymentDateEdit, L"");
}

int MainForm::Run(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"SportsClubManagerClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Менеджер спортивного клубу",
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