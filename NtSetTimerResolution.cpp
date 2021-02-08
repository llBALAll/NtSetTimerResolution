#include <windows.h>
#include <stdio.h>
#include <conio.h>

extern "C" NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);
extern "C" NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
#define STATUS_SUCCESS 0x00000000
#define STATUS_TIMER_RESOLUTION_NOT_SET 0xC0000245

BOOLEAN isCurrentResolutionChanged = FALSE;
ULONG CurrentResolution = 0;
ULONG DesiredResolution = 5000;//5ms

int main() {

    NTSTATUS ntStatus = STATUS_SUCCESS;
    ULONG MinimumResolution = 0;
    ULONG MaximumResolution = 0;

    HWND hwnd;

    if((ntStatus = NtQueryTimerResolution(&MinimumResolution, &MaximumResolution, &CurrentResolution)) != STATUS_SUCCESS) {
        printf("Something wrong with NtQueryTimerResolution call.");
        return ntStatus;
    }

//	printf("MinimumResolution [100 ns units]: %d.\n", MinimumResolution);
//	printf("MaximumResolution [100 ns units]: %d.\n", MaximumResolution);
//	printf("CurrentResolution [100 ns units]: %d.\n", CurrentResolution);
    printf("MinimumResolution [ms]: %f.\n", (float)MinimumResolution / 10000);
    printf("MaximumResolution [ms]: %f.\n", (float)MaximumResolution / 10000);
    printf("CurrentResolution [ms]: %f.\n", (float)CurrentResolution / 10000);

    if ((ntStatus = NtSetTimerResolution(DesiredResolution, TRUE, &CurrentResolution)) != STATUS_SUCCESS)
        printf("Something wrong with NtSetTimerResolution call.");
    else {
        isCurrentResolutionChanged = TRUE;
//		printf("New CurrentResolution [100 ns units]: %d.\n", CurrentResolution);
		printf("New CurrentResolution [ms]: %f.\n", (float)CurrentResolution / 10000);
//        printf("Press any key to restore the original resolution...\n");
        printf("This app will be invisible..if you want to quit use task manager to end the process...\n");
        system("pause");
		//FreeConsole();
        hwnd=GetConsoleWindow();
		ShowWindow(hwnd,SW_HIDE);
		system("pause");
/*
        switch ((ntStatus = NtSetTimerResolution(DesiredResolution, FALSE, &CurrentResolution))) {
            case STATUS_SUCCESS:
//				printf("The current resolution has returned to %d [100 ns units].\n", CurrentResolution);
                printf("CurrentResolution [ms]: %f.\n", (float)CurrentResolution / 10000);
                break;
            case STATUS_TIMER_RESOLUTION_NOT_SET:
                printf("The requested resolution was not set by this application.\n");
                break;
            default:
                printf("Something wrong with NtSetTimerResolution call.");
        }
*/
    }

    return ntStatus;
}
