// Scheduler data structure for storing task data
typedef struct
{
   // Pointer to task
   void (* pTask)(void);
   // Initial delay in ticks
   unsigned int Delay;
   // Periodic interval in ticks
   unsigned int Period;
   // Runme flag (indicating when the task is due to run)
   unsigned char RunMe;
} sTask;

// Function prototypes
//-------------------------------------------------------------------

void initSCH(void);
void SCHStart(void);
// Core scheduler functions
void SCHDispatchTasks(void);
unsigned char SCHAddTask(void (*)(void), const unsigned int, const unsigned int);
unsigned char SCHDeleteTask(const unsigned char);

#define SCH_MAX_TASKS (5)
