#include "MainForm.h"
#include "SportsClubManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SportsClubManager manager;
    MainForm form(&manager);
    return form.Run(hInstance);
}
