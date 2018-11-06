#include <stdio.h>
#define NUMBANDS  15
#define GAP  10
#define Q_CONSTANT 1

struct band {
  float gain;
  float frequency;
  float width;
};
void add_beginning(FILE *output)
{
  fprintf(output, "{\n  \"equalizer\": {");
  fprintf(output, "\n   \"state\": \"true\",");
  fprintf(output, "\n   \"num-bands\": \"%d\",", NUMBANDS);
  fprintf(output, "\n   \"input-gain\": \"0\",");
  fprintf(output, "\n   \"output-gain\": \"0\",");
}
void add_band(FILE *output, int bandNum, struct band this_band)
 {
  fprintf(output, "\n   \"band%d\": {", bandNum);
  fprintf(output, "\n     \"gain\": \"%f\",", this_band.gain);
  fprintf(output, "\n     \"frequency\": \"%f\",", this_band.frequency);
  fprintf(output, "\n     \"width\": \"%f\",", this_band.width);
  fprintf(output, "\n     \"type\": \"peak\"");
  fprintf(output, "\n   },");
}

int read_bands(FILE *in, struct band bands[], int *counter)
{
  float freq, gain;
  // skip 2 lines
  fscanf(in, "%*[^\n]\n", NULL);
  fscanf(in, "%*[^\n]\n", NULL);

  while (*counter < 230 && fscanf(in,"%f %f", &freq, &gain) == 2) {
    struct band bandTemp;
    bandTemp.gain = gain;
    bandTemp.frequency = freq;
    bandTemp.width = Q_CONSTANT;
    bands[*counter] = bandTemp;
    printf("\nInter num %d, %f %f", *counter, bandTemp.frequency, bandTemp.gain);
    (*counter)++;
  }
}

void main()
{
  int counter = 0, counter2 = 0;
  FILE *frd, *preset;
  struct band bands[230];

  frd = fopen("Volume40FeetTouchEQ.frd", "r");
  preset = fopen ("preset.json", "w+");

  add_beginning(preset);
  read_bands(frd, bands, &counter);
  for (int i = 0; i < 220; i = i + GAP)
  {
    add_band(preset, counter2, bands[i]);
    counter2++;
  }
  fprintf(preset, "\n}");
  fclose(frd);
  fclose(preset);
}
