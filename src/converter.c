#include <stdio.h>
#define NUMBANDS  22
#define GAP  10
#define Q_CONSTANT 10

struct band {
  float gain;
  float frequency;
  float width;
};
void add_beginning(FILE *beginning, FILE *output)
{
  char c;
  // copy the non eq parts of the preset into the output
  while ((c = fgetc(beginning)) != EOF) {
    fputc(c, output);
  }
  fprintf(output, "        \"equalizer\": {");
  fprintf(output, "\n            \"state\": \"true\",");
  fprintf(output, "\n            \"num-bands\": \"%d\",", NUMBANDS);
  fprintf(output, "\n            \"input-gain\": \"0\",");
  fprintf(output, "\n            \"output-gain\": \"0\",");
}

void add_band(FILE *output, int bandNum, struct band this_band, int end)
 {
  fprintf(output, "\n            \"band%d\": {", bandNum);
  if (this_band.gain >= 12) {
    fprintf(output, "\n                \"gain\": \"%f\",", (float)12);
  }
  else if (this_band.gain <=-24) {
    fprintf(output, "\n                \"gain\": \"%f\",", (float)-24);
  }
  else {
    fprintf(output, "\n                \"gain\": \"%f\",", this_band.gain);
  }
  fprintf(output, "\n                \"frequency\": \"%f\",", this_band.frequency);
  fprintf(output, "\n                \"width\": \"%f\",", this_band.width);
  fprintf(output, "\n                \"type\": \"peak\"");
  if (end) {
    fprintf(output, "\n            }");
  }
  else {
    fprintf(output, "\n           },");
  }
}

void end_file(FILE *end, FILE *output)
{
  char c;
  fprintf(output, "\n        },");
  while ((c = fgetc(end)) != EOF) {
    fputc(c, output);
  }
  // fprintf(output, "\n }\n}");
}

void calculate_widths(struct band bands[])
{
   bands[0].width = 3; // bad number
   for (int i = 1; i < NUMBANDS; i++)
   {
	float prev, next;
	prev = bands[i].frequency - bands[i - 1].frequency;
	if (i != NUMBANDS - 1)
	{
		next = bands[i + 1].frequency - bands[i].frequency;

		if (next >= prev)
		{
		   bands[i].width = prev / 2;
		}
		else
		{
		   bands[i].width = next / 2;
		}
	}
	else
	{
		bands[i].width = prev / 2;
	}

   }
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
  FILE *frd, *beginning, *end, *preset;
  struct band bands[230], final_bands[NUMBANDS];

  frd = fopen("Volume40FeetTouchEQ.frd", "r");
  beginning = fopen("baseline_beginning.json", "r");
  end = fopen("baseline_ending.json", "r");
  preset = fopen ("preset.json", "w");

  add_beginning(beginning, preset);
  read_bands(frd, bands, &counter);

  for (int i = 0; i < 220; i = i + GAP)
  {
    final_bands[counter2] = bands[i];//add_band(preset, counter2, bands[i], 0);
    counter2++;
  }
  calculate_widths(final_bands);
  for (int i = 0; i < NUMBANDS; i++)
	{
		  if (i + 1  != NUMBANDS) {
     		 add_band(preset, i, final_bands[i], 0);
  		  }
   		 else {
    		  add_band(preset, i, final_bands[i], 1);
  		  }
	}
  end_file(end, preset);

  fclose(frd);
  fclose(preset);
}
