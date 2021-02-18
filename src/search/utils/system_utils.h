#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <string>

class SystemUtils {
public:
    static void abort(std::string const& message);

    static bool readFile(std::string& file, std::string& res,
                         std::string ignoreSign = "");
#ifndef __APPLE__
    static long getTotalVirtualMemory();
    static long getUsedVirtualMemory();
    static int getVirtualMemoryUsedByThis();

    static long getTotalRAM();
    static long getUsedRAM();

    static void initCPUMeasurementOfThis();
    static double getCPUUsageOfThis();
#endif
    static int getRAMUsedByThis();
protected:

    static bool CPUMeasurementRunning;
    static bool CPUMeasurementOfProcessRunning;

    static clock_t lastCPU;
    static clock_t lastSysCPU;
    static clock_t lastUserCPU;
    static int numProcessors;

    static int parseLine(char* line);
private:
    SystemUtils() {}
};

#endif
