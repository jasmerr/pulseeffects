Usage instructions:
Compile the program - "gcc -o converter converter.c"

Run the program - "./converter example.frd". An argument containing the name of the
desired .frd must be included or it will fail to run.

This program will skip over the first two lines of text in the FRD file.
This is because the OmniMic software includes informational text on these lines
by default when exporting an eq.frd.

Output - "preset.json" by default.

This preset file can be loaded into PulseEffects without any errors in my tests.
