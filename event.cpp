#include "nssm.h"

static char error_message[65535];

/* Convert error code to error string - must call LocalFree() on return value */
char *error_string(unsigned long error) {
  if (! FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *) &error_message, sizeof(error_message), 0)) {
    if (_snprintf(error_message, sizeof(error_message), "system error %lu", error) < 0) return 0;
  }
  return error_message;
}

/* Log a message to the Event Log */
void log_event(unsigned short type, unsigned long id, ...) {
  va_list arg;
  int count;
  char *s;
  char *strings[6];

  /* Open event log */
  HANDLE handle = RegisterEventSource(0, TEXT(NSSM));
  if (! handle) return;

  /* Log it */
  count = 0;
  va_start(arg, id);
  while ((s = va_arg(arg, char *))) strings[count++] = s;
  va_end(arg);
  ReportEvent(handle, type, 0, id, 0, count, 0, (const char **) strings, 0);

  /* Close event log */
  DeregisterEventSource(handle);
}
