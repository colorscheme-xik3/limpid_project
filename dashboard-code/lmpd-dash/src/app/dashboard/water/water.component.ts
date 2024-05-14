import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { AngularFireDatabase } from '@angular/fire/compat/database';
import { Observable, BehaviorSubject } from 'rxjs';
import { switchMap, startWith, distinctUntilChanged } from 'rxjs/operators';
import ApexCharts from 'apexcharts';

interface SensorValues {
  [key: string]: {
    Temperature: string;
    "Potential Hydrogen": string;
    "Total Dissolved Solids": string;
    "Dissolved Oxygen": string;
    "Turbidity": string;
    "Water Type": string;
  };
}


@Component({
  selector: 'app-water',
  templateUrl: './water.component.html',
  styleUrls: ['./water.component.css']
})


export class WaterComponent implements OnInit {
  sensorValues$: Observable<SensorValues> | null = null;
  private latestSensorValues: SensorValues | null = null;  lastTimestamp: string[] | null = null;
  lastFiveTimestamps: string[] | null = null;
  private sensorValuesSubject = new BehaviorSubject<SensorValues | null>(null);  
  private chart: ApexCharts | null = null;
  private chartRadar: ApexCharts | null = null;
  showTemperature: boolean = true; // Variable to control temperature visibility
  showPotentialHydrogen: boolean = true; // Variable to control potential hydrogen visibility
  showTDS: boolean = true; // Variable to control potential hydrogen visibility
  showDO: boolean = true; // Variable to control potential hydrogen visibility
  showTurb: boolean = true; // Variable to control potential hydrogen visibility

  public showAreaChart: boolean = false; // Flag to control which chart to display

  selectedWaterType: string = ''; // Default selected water type

  timestamps: string = ''; // Declare timestamps property here
  selectedDuration: string = ''; // Initialized with an empty string

  constructor(private db: AngularFireDatabase, private afAuth: AngularFireAuth) {}

  ngOnInit() {

    
    this.showAreaChart = false; // Start with the area chart

    if (!this.selectedDuration) {
      this.selectedDuration = '5'; // Set default value if selectedDuration is empty
      this.selectedWaterType = 'TAP WATER'; // Set default value if selectedDuration is empty

    }
  
    console.log('Initializing WaterComponent');
    this.sensorValues$ = this.afAuth.authState.pipe(
      switchMap(user => {
        if (user) {
          console.log('User is logged in:', user.uid);
          // If the user is logged in, fetch sensor values
          return this.getData(`/users/${user.uid}/sensorValues`);
        } else {
          console.log('User is not logged in');
          // If the user is not logged in, return an observable with null
          return new Observable();
        }
      }),
      startWith(null),
      distinctUntilChanged()
    );

    this.sensorValues$.subscribe(sensorValues => {
      console.log('Received sensorValues:', sensorValues);
    
      // Store the latest sensorValues
      this.latestSensorValues = sensorValues || null;
      this.sensorValuesSubject.next(sensorValues || null);
    
      // Get the last timestamp when sensorValues are received
      this.lastTimestamp = this.getLastTimestamps(sensorValues, 1);
      
      // Check if this.lastFiveTimestamps is not null before using it
      if (this.lastFiveTimestamps !== null) {
        const selectedDuration = parseInt(this.selectedDuration, 10);
        
        // Compute average values for the last N timestamps based on selected duration
        const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, this.lastFiveTimestamps);
        
        // Update the radar chart with the computed average values
        this.updateRadarChart(averageValues);
      }
    
      // Update the chart when sensorValues are received
      this.toggleChartType();
    });
    
  }


  private computeAverageValuesAcrossTimestamps(sensorValues: SensorValues, timestamps: string[]): number[] {
    const totalValues: number[] = [0, 0, 0, 0, 0]; // Initialize total values array
    let count = 0; // Initialize count of valid entries

    timestamps.forEach(timestamp => {
      const entry = sensorValues[timestamp];
      if (entry) {
        totalValues[0] += parseFloat(entry.Temperature) || 0;
        totalValues[1] += parseFloat(entry["Potential Hydrogen"]) || 0;
        totalValues[2] += parseFloat(entry["Total Dissolved Solids"]) || 0;
        totalValues[3] += parseFloat(entry["Dissolved Oxygen"]) || 0;
        totalValues[4] += parseFloat(entry["Turbidity"]) || 0;
        count++;
      }
    });

    // Calculate averages
    const averages = totalValues.map(value => (count > 0 ? value / count : 0));
    return averages;
  }




  

  redoAreaChart(): void {

    this.updateAreaChart(); // Update with area chart data
 
  }


  toggleChartType(): void {
    this.showAreaChart = !this.showAreaChart; // Toggle the chart type flag
    console.log('showAreaChart:', this.showAreaChart); // Log the current state of showAreaChart
  
    if (this.showAreaChart) {
      // Update the chart to display area chart
      this.initializeChart();
      this.updateAreaChart(); // Update with area chart data
    } else {
      // Update the chart to display radar chart
      if (this.latestSensorValues) {
        // Determine the timestamps based on the selected duration
        const selectedDuration = parseInt(this.selectedDuration, 10);
        const timestamps = this.getLastTimestamps(this.latestSensorValues, selectedDuration);
  
        if (timestamps) {
          // Compute average values based on the filtered timestamps
          const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, timestamps);
          
          // Initialize and update radar chart with computed average values
          this.initializeRadar(averageValues);
          this.updateRadarChart(averageValues);
  
          if (timestamps.length === 0) {
            console.warn('No timestamps available for the selected duration.');
          }
        } else {
          console.warn('No timestamps available for the selected duration.');
        }
      } else {
        console.warn('Not enough data to initialize the radar chart.');
      }
    }
  }
  


onDurationChange(): void {
  console.log('Selected Duration:', this.selectedDuration);
  
  if (!this.latestSensorValues) {
    console.warn('No sensorValues available.');
    return;
  }

  // Update lastFiveTimestamps based on the selected duration
  const selectedDuration = parseInt(this.selectedDuration, 10);
  this.lastFiveTimestamps = this.getLastTimestamps(this.latestSensorValues, selectedDuration);

  // Determine which chart to update based on the current chart type
  if (this.showAreaChart) {
    // Update area chart with new data
    this.updateAreaChart();
  } else {
    // Compute average values based on the latest sensor values and lastFiveTimestamps
    if (this.lastFiveTimestamps && this.lastFiveTimestamps.length > 0) {
      const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, this.lastFiveTimestamps);
      this.updateRadarChart(averageValues);
    } else {
      console.warn('No timestamps available for the selected duration.');
    }
  }
}

  

  onWaterTypeChange(): void {
    console.log('Selected Water Type:', this.selectedWaterType);
    this.updateAreaChart(); // Update chart data based on selected water type
  }
  
  

  updateAreaChart() {
    if (this.chart && this.lastFiveTimestamps && this.sensorValuesSubject) {
      const filteredSensorValues = this.lastFiveTimestamps.map(timestamp => {
        const sensorValue = this.sensorValuesSubject.value;
        const entry: {
          Timestamp: string;
          Temperature?: number;
          "Potential Hydrogen"?: number;
          "Total Dissolved Solids"?: number;
          "Dissolved Oxygen"?: number;
          "Turbidity"?: number;
        } = { Timestamp: timestamp };
  
        // Check if sensorValue exists and matches the selected water type
        if (sensorValue && sensorValue[timestamp]?.["Water Type"] === this.selectedWaterType) {
          // Include sensor values only if they match the selected water type
          if (this.showTemperature) {
            entry.Temperature = parseFloat(sensorValue[timestamp]?.Temperature);
          }
          if (this.showPotentialHydrogen) {
            entry["Potential Hydrogen"] = parseFloat(sensorValue[timestamp]?.['Potential Hydrogen']);
          }
          if (this.showTDS) {
            entry["Total Dissolved Solids"] = parseFloat(sensorValue[timestamp]?.['Total Dissolved Solids']);
          }
          if (this.showDO) {
            entry["Dissolved Oxygen"] = parseFloat(sensorValue[timestamp]?.['Dissolved Oxygen']);
          }
          if (this.showTurb) {
            entry["Turbidity"] = parseFloat(sensorValue[timestamp]?.['Turbidity']);
          }
        }
  
        return entry;
      });
  
      // Define the series type explicitly
      interface Series {
        name: string;
        data: (number | null)[];
      }
  
      // Ensure both series are present in the chart data
      const series: Series[] = [];
      if (this.showTemperature) {
        series.push({
          name: 'Temperature',
          data: filteredSensorValues.map(entry => entry.Temperature ?? null),
        });
      }
      if (this.showPotentialHydrogen) {
        series.push({
          name: 'Potential Hydrogen',
          data: filteredSensorValues.map(entry => entry["Potential Hydrogen"] ?? null),
        });
      }
      if (this.showTDS) {
        series.push({
          name: 'Total Dissolved Solids',
          data: filteredSensorValues.map(entry => entry["Total Dissolved Solids"] ?? null),
        });
      }
      if (this.showDO) {
        series.push({
          name: 'Dissolved Oxygen',
          data: filteredSensorValues.map(entry => entry["Dissolved Oxygen"] ?? null),
        });
      }
      if (this.showTurb) {
        series.push({
          name: 'Turbidity',
          data: filteredSensorValues.map(entry => entry["Turbidity"] ?? null),
        });
      }
  
      this.chart.updateSeries(series.filter(s => s.data.some(val => val !== null))); // Filter out series with all null values
  
      // Update the x-axis labels
      this.chart.updateOptions({
        xaxis: {
          categories: filteredSensorValues.map(entry => entry.Timestamp),
        },
      });
  
      // Update the data labels based on checked checkboxes
      this.chart.updateOptions({
        dataLabels: {
          enabledOnSeries: series.map((s, index) => index).filter(i => series[i].data.some(val => val !== null)),
        },
      });
    } else {
      // Handle the case where there are no timestamps or not enough timestamps to update the chart
      console.warn('Not enough timestamps to update the chart.');
    }
  }

   updateRadarChart(highestValues: number[]): void {
    if (this.chartRadar && this.sensorValuesSubject) {
      const categories = ['Temperature', 'Potential Hydrogen', 'Total Dissolved Solids', 'Dissolved Oxygen', 'Turbidity', 'Water Type'];
  
      // Map the highest values to the corresponding categories
      const data = highestValues.map((value, index) => {
        return { x: categories[index], y: value };
      });
  
      const series = [{ name: 'Highest Values', data: data }];
  
      this.chartRadar.updateSeries(series);
      this.chartRadar.updateOptions({
        xaxis: {
          categories: categories,
        },
      });
    } else {
      console.warn('Not enough data to update the radar chart.');
    }
  }
  
  
  
  

  private getData(path: string): Observable<any> {
    return this.db.object<any>(path).valueChanges();
  }

  private getLastTimestamps(sensorValues: { [key: string]: { Temperature: string, "Potential Hydrogen": string } }, n: number): string[] | null {
    const timestamps = Object.keys(sensorValues || {});
    const length = timestamps.length;

    if (length >= n) {
      return timestamps.slice(-n);
    } else if (length === 1) {
      // If there's only one timestamp, return an array with the same timestamp (no nth-to-last)
      return [timestamps[0]];
    } else {
      // No timestamps or not enough timestamps
      return null;
    }
  }

  private initializeChart() {
    if (!this.chart) {
      // Create an ApexCharts instance for the chart
      this.chart = new ApexCharts(document.getElementById('chart1'), this.getChartOptions());
      // Render the chart
      this.chart.render();
    }
  }

  private initializeRadar(highestValues: number[]): void {
    if (!this.chartRadar) {
      // Create an ApexCharts instance for the chart
      this.chartRadar = new ApexCharts(document.getElementById('chart1'), this.getRadarOptions(highestValues));
      // Render the chart
      this.chartRadar.render();
    }
  }

  private getChartOptions(): ApexCharts.ApexOptions {
    const seriesColors = [];
    const dataLabelColors = [];
    const yAxisColors = ['#FFFFFF']; // Default color for first y-axis
  
    // Configure colors and styles based on sensor visibility
    if (this.showTemperature) {
      seriesColors.push('#FFFFFF'); // Green color for Temperature series
      dataLabelColors.push('#000000'); // Label color for Temperature series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)

    }
    if (this.showPotentialHydrogen) {
      seriesColors.push('#509BA8'); // Yellow color for Potential Hydrogen series
      dataLabelColors.push('#509BA8'); // Label color for Potential Hydrogen series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)

    }
    if (this.showTDS) {
      seriesColors.push('#E22800'); // Red color for Total Dissolved Solids series
      dataLabelColors.push('#E22800'); // Label color for Total Dissolved Solids series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)

    }
    if (this.showDO) {
      seriesColors.push('#F4BB00'); // Blue color for Dissolved Oxygen series
      dataLabelColors.push('#F4BB00'); // Label color for Dissolved Oxygen series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)
    }
    if (this.showTurb) {
      seriesColors.push('#65469E'); // Orange color for Turbidity series
      dataLabelColors.push('#65469E'); // Label color for Turbidity series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Turbidity)
    }
  
    return {
      series: [],
      chart: {
        height: 450,
        type: 'area',
        toolbar: {
          show: false,
        },
      },
      colors: seriesColors,
      dataLabels: {
        enabled: true,
        style: {
          colors: dataLabelColors,
        },
      },
      stroke: {
        curve: 'smooth',
      },
      labels: [],
      markers: {
        size: 0,
      },
      xaxis: {
        labels: {
          style: {
            colors: '#FFFFFF',
          },
        },
      },
      yaxis: [
        {
          title: {
            style: {
              color: yAxisColors[0], // Color for first y-axis
            },
          },
          labels: {
            style: {
              colors: yAxisColors[0], // Color for first y-axis
            },
          },
        },
        {
          opposite: true,
          title: {
            style: {
              color: yAxisColors[1] || '#FFFFFF', // Color for second y-axis (or default color)
            },
          },
          labels: {
            style: {
              colors: yAxisColors[1] || '#FFFFFF', // Color for second y-axis (or default color)
            },
          },
        },
      ],
      tooltip: {
        shared: true,
        intersect: false,
        style: {
          fontSize: '12px',
        },
        theme: 'dark',
      },
      legend: {
        labels: {
          colors: '#FFFFFF', // Text color for legend items
        },
      },
    };
    
  }


  private getRadarOptions(highestValues: number[]): ApexCharts.ApexOptions {
    const dataLabelColors = ['#000000'];
  
    return {
      series: [
        {
          name: 'Series 1',
          data: highestValues  // Use the computed highest values for radar chart data
        }
      ],
      chart: {
        height: 450,
        type: 'radar'
      },
      xaxis: {
        categories: ['Temperature', 'Potential Hydrogen', 'Total Dissolved Solids', 'Dissolved Oxygen', 'Turbidity', 'Water Type'],
        labels: {
          style: {
            colors: '#FFFFFF'
          }
        }
      },
      yaxis: {
        labels: {
          style: {
            colors: '#FFFFFF'
          }
        }
      },
      markers: {
        size: 0
      },
      dataLabels: {
        enabled: true,
        style: {
          colors: dataLabelColors
        }
      },
      tooltip: {
        shared: true,
        intersect: false,
        style: {
          fontSize: '12px'
        },
        theme: 'dark'
      },
      legend: {
        labels: {
          colors: '#FFFFFF'
        }
      }
    };
  }
    
}




  