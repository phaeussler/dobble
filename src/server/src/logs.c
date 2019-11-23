#include <logs.h>

#define NAME_SIZE 65
#define MESG_SIZE 512

extern FILE *LOGGER;

DEBUGGER = 0;
INFO = 1;
WARNING = 1;
ERROR = 1;
CRITIC = 1;

extern const char *MESAGE_NAMES[NAME_SIZE];

MESAGE_NAMES[0] = "Not Defined";
MESAGE_NAMES[1] = "Start Connection";
MESAGE_NAMES[2] = "Connection Established";
MESAGE_NAMES[3] = "Ask Nickname";
MESAGE_NAMES[4] = "Return Nickname";
MESAGE_NAMES[5] = "Opponent Found";
MESAGE_NAMES[6] = "Send IDs";
MESAGE_NAMES[7] = "Start Game";
MESAGE_NAMES[8] = "Scores";
MESAGE_NAMES[9] = "Send Cards";
MESAGE_NAMES[10] = "Send Word";
MESAGE_NAMES[11] = "Response Word";
MESAGE_NAMES[12] = "Round Winner/Loser";
MESAGE_NAMES[13] = "End Game"; // Tu Tuuu TuTuuuuuu Tuuuuuuuuuuuuuuuu Tuuuuuuuuuuuuuuuuuu Tuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
MESAGE_NAMES[14] = "Game Winner/Loser";
MESAGE_NAMES[15] = "Ask New Game";
MESAGE_NAMES[16] = "Answer New Game";
MESAGE_NAMES[17] = "Disconnect";
MESAGE_NAMES[18] = "Not defined";
MESAGE_NAMES[19] = "Not defined";
MESAGE_NAMES[20] = "Error Bad Package";
MESAGE_NAMES[21] = "Not Defined";
MESAGE_NAMES[22] = "Not Defined";
MESAGE_NAMES[23] = "Not Defined";
MESAGE_NAMES[24] = "Not Defined";
MESAGE_NAMES[25] = "Not Defined";
MESAGE_NAMES[26] = "Not Defined";
MESAGE_NAMES[27] = "Not Defined";
MESAGE_NAMES[28] = "Not Defined";
MESAGE_NAMES[29] = "Not Defined";
MESAGE_NAMES[30] = "Not Defined";
MESAGE_NAMES[31] = "Not Defined";
MESAGE_NAMES[32] = "Not Defined";
MESAGE_NAMES[33] = "Not Defined";
MESAGE_NAMES[34] = "Not Defined";
MESAGE_NAMES[35] = "Not Defined";
MESAGE_NAMES[36] = "Not Defined";
MESAGE_NAMES[37] = "Not Defined";
MESAGE_NAMES[38] = "Not Defined";
MESAGE_NAMES[39] = "Not Defined";
MESAGE_NAMES[40] = "Not Defined";
MESAGE_NAMES[41] = "Not Defined";
MESAGE_NAMES[42] = "Not Defined";
MESAGE_NAMES[43] = "Not Defined";
MESAGE_NAMES[44] = "Not Defined";
MESAGE_NAMES[45] = "Not Defined";
MESAGE_NAMES[46] = "Not Defined";
MESAGE_NAMES[47] = "Not Defined";
MESAGE_NAMES[48] = "Not Defined";
MESAGE_NAMES[49] = "Not Defined";
MESAGE_NAMES[50] = "Not Defined";
MESAGE_NAMES[51] = "Not Defined";
MESAGE_NAMES[52] = "Not Defined";
MESAGE_NAMES[53] = "Not Defined";
MESAGE_NAMES[54] = "Not Defined";
MESAGE_NAMES[55] = "Not Defined";
MESAGE_NAMES[56] = "Not Defined";
MESAGE_NAMES[57] = "Not Defined";
MESAGE_NAMES[58] = "Not Defined";
MESAGE_NAMES[59] = "Not Defined";
MESAGE_NAMES[60] = "Not Defined";
MESAGE_NAMES[61] = "Not Defined";
MESAGE_NAMES[62] = "Not Defined";
MESAGE_NAMES[63] = "Not Defined";
MESAGE_NAMES[64] = "Image";

void setup_logger(char *fp, int op)
{
    if (op)
    {
        LOGGER = fopen(fp, "a+"); // En caso de que también queramos leer
    }
    else
    {
        LOGGER = stdout;
    }
}

void debuger(char *msg)
{
    if (DEBUGGER)
    {
        char message[MESG_SIZE];
        sprintf(message, "|[ __DEBUG__ ]|>> %s <<|", msg);
        log(message);
    }
}

void infolog(char *msg)
{
    if (INFO)
    {
        char message[MESG_SIZE];
        sprintf(message, "|[ __INFO__ ]|>> %s <<|", msg);
        log(message);
    }
} // Nivel de los logs solicitados

void warning(char *msg)
{
    if (WARNING)
    {
        char message[MESG_SIZE];
        sprintf(message, "|[ __WARNING__ ]|>> %s <<|", msg);
        log(message);
    }
}

void error(char *msg)
{
    if (ERROR)
    {
        char message[MESG_SIZE];
        sprintf(message, "|[ __ERROR__ ]|>> %s <<|", msg);
        log(message);
    }
}

void critical(char *msg)
{
    if (CRITIC)
    {
        char message[MESG_SIZE];
        sprintf(message, "|[ __CRITICAL__ ]|>> %s <<|", msg);
        log(message);
    }
}

void log(char *msg)
{
    char buf[30];
    fprintf(LOGGER, "\n[ SERVER:: %s ]%s\n", get_time_str(buf), msg);
}

void close_logger(int op)
{
    if (op)
        fclose(LOGGER);
}

char *get_time_str(char *buf)
{
    struct timeval tmnow;
    struct tm *tm;
    char usec_buf[6];
    gettimeofday(&tmnow, NULL);
    tm = localtime(&tmnow.tv_sec);
    strftime(buf, 30, "%Y/%m/%d-%H:%M:%S", tm);
    strcat(buf, ".");
    sprintf(usec_buf, "%d", (int)tmnow.tv_usec);
    strcat(buf, usec_buf);
    return buf;
}
