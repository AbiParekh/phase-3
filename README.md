# phase-3

Run program: Ctrl + F5 or Debug > Start Without Debugging menu
Debug program: F5 or Debug > Start Debugging menu

Tips for Getting Started: 
  1. Use the Solution Explorer window to add/manage files
  2. Use the Team Explorer window to connect to source control
  3. Use the Output window to see build output and other messages
  4. Use the Error List window to view errors
  5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
  6. In the future, to open this project again, go to File > Open > Project and select the .sln file


Running Program from CMD Terminal
  1. Open "config.txt" and adjust the following default parameters as needed to point to the correct directories and run the appropriate threads:
        Input_Directory ..\inputDir\
        Map_DLL_Location .\..\MapDLL\MapLibrary\MapLibrary.dll
        Reduce_DLL_Location .\..\ReduceDLL\ReduceLib\ReduceLib.dll
        Number_Of_Map_Threads 5
        Number_Of_Reduce_Threads 4
        Map_Buffer_Size 3000
        
  3. In terminal change directory to path of executable:
        cd \phase-3\MapReduceWF\MapReduceWF.exe
        
  4. Run program passing config file as argument i.e.:   
        MapReduceWF.exe ..\config.txt
        
  5. If execution passes the debug messages will print:
        "Successfully Reduced Input Directory into the file: FinalOutput.txt!"
