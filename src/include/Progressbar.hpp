/*  The MIT License (MIT)
    ============================================================================
    Author: Blackflame576
    ============================================================================
    Created: 4 Juny 2023
    ============================================================================
*/
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

namespace Bar
{
    int Process;

    class ProgressBar_v1
    {
    public:
        string doneSymbol = "#";
        string todoSymbol = " ";
        string startSymbol = "[";
        string endSymbol = "]";
        string Output;
        int maxSymbols = 25;
        int n_done = 0;
        string EmptyStr;
        int LastSizeStr;
        string OutputStr;

        void Update(double DownloadedSize = 0.0, double TotalSize = 0.0)
        {
            Process += 1;
            Output = startSymbol;
            for (int i = 0; i < n_done; i++)
            {
                Output += doneSymbol;
            }
            if (Process % 4 == 0)
            {
                Output += doneSymbol;
                n_done += 1;
            }
            if (maxSymbols - n_done)
            {
                for (int i = 0; i < (maxSymbols - n_done); i++)
                {
                    Output += todoSymbol;
                }
            }
            Output += endSymbol;
            EmptyStr = "";
            for (int i = 0; i < LastSizeStr; i++)
            {
                EmptyStr += todoSymbol;
            }
            cout << "\r" << EmptyStr << flush;
            OutputStr = Output + " " + to_string(Process) + "%  ";
            if (DownloadedSize != 0.0  && TotalSize != 0.0)
            {
                OutputStr = OutputStr + AutoConvertSize(DownloadedSize) + " / " + AutoConvertSize(TotalSize);
            }
            cout << "\r" << OutputStr << flush;
            LastSizeStr = OutputStr.size();
        }
        void ResetAll()
        {
            cout << "" << endl;
            Process = 0;
            LastSizeStr = 0;
            OutputStr = "";
            EmptyStr = "";
            Output = "";
            n_done = 0;
        }

    private:
        string round(double value)
        {
            // Print value to a string
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << value;
            std::string str = ss.str();
            // Ensure that there is a decimal point somewhere (there should be)
            if (str.find('.') != std::string::npos)
            {
                // Remove trailing zeroes
                str = str.substr(0, str.find_last_not_of('0') + 1);
                // If the decimal point is now the last character, remove that as well
                if (str.find('.') == str.size() - 1)
                {
                    str = str.substr(0, str.size() - 1);
                }
            }
            return str;
        }

        string AutoConvertSize(float Size)
        {
            string ConvertedSize;
            Size = convert_to_KB(Size);
            if (Size >= 1024)
            {
                ConvertedSize = round(convert_to_MB(Size)) + " MB";
            }
            else if (convert_to_MB(Size) >= 1024)
            {
                ConvertedSize = round(convert_to_GB(Size)) + " GB";
            }
            else
            {
                ConvertedSize = round(Size) + " KB";
            }
            return ConvertedSize;
        }

        double convert_to_MB(double Value)
        {
            double NewValue = Value / 1024;
            return NewValue;
        }

        double convert_to_KB(double Value)
        {
            double NewValue = Value / 1024;
            return NewValue;
        }

        double convert_to_GB(double Value)
        {
            double NewValue = Value / 1024;
            return NewValue;
        }
    };

    class ProgressBar_v2
    {
    public:
        string ProgressSymbols[4] = {"/","-","\\","|"};
        int n = 0;
        void Update()
        {
            cout << "\r" << ProgressSymbols[n] << flush;
            if (n == (end(ProgressSymbols) - begin(ProgressSymbols) - 1)) n = 0;
            else n++;
        }

    private:
    };
}
#endif