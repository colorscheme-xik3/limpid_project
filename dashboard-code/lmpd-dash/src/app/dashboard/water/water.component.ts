import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { AngularFireDatabase } from '@angular/fire/compat/database';
import { Observable, BehaviorSubject } from 'rxjs';
import { switchMap, startWith, distinctUntilChanged } from 'rxjs/operators';
import ApexCharts from 'apexcharts';
import { ApexOptions } from 'apexcharts';
import { time } from 'console';

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
  private   chartRadar: ApexCharts | null = null;
  showTemperature: boolean = true; // Variable to control temperature visibility
  showPotentialHydrogen: boolean = true; // Variable to control potential hydrogen visibility
  showTDS: boolean = true; // Variable to control potential hydrogen visibility
  showDO: boolean = true; // Variable to control potential hydrogen visibility
  showTurb: boolean = true; // Variable to control potential hydrogen visibility

  public showAreaChart: boolean = false; // Flag to control which chart to display

  selectedWaterType: string = ''; // Default selected water type

  timestamps: string = ''; // Declare timestamps property here
  selectedDuration: string = ''; // Initialized with an empty string
  chartKey: number = 0; // Key to force re-render of the chart
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
        const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, this.lastFiveTimestamps, this.selectedWaterType);
        
        // Update the radar chart with the computed average values
       this.updateRadarChart(averageValues);
      }
    
      // Update the chart when sensorValues are received
      this.checkChartType();
    });
    
  }


  private computeAverageValuesAcrossTimestamps(sensorValues: SensorValues, timestamps: string[], waterType: string): number[] {
    const totalValues: number[] = [0, 0, 0, 0]; // Initialize total values array
    let count = 0; // Initialize count of valid entries
  
    timestamps.forEach(timestamp => {
      const entry = sensorValues[timestamp];
      // Exclude entries with water type "FLOATING PROBE"
      if (entry && entry["Water Type"]!== 'FLOATING PROBE') {
        if (entry["Water Type"] === waterType) {
          totalValues[0] += parseFloat(entry.Temperature) || 0;
          totalValues[1] += parseFloat(entry["Potential Hydrogen"]) || 0;
          totalValues[2] += parseFloat(entry["Total Dissolved Solids"]) || 0;
          totalValues[3] += parseFloat(entry["Dissolved Oxygen"]) || 0;
          count++;
        }
      }
    });
  
    // Calculate averages
    const averages = totalValues.map(value => (count > 0? value / count : 0));
    return averages;
  }
  
  
  redoAreaChart(): void {

    this.updateAreaChart(); // Update with area chart data
 
  }

  checkChartType(): void {
    this.chartKey++;

    if (this.showAreaChart) {
      // Destroy the radar chart if it exists
      if (this.chartRadar) {
        this.chartRadar.destroy();
        this.chartRadar = null;
      }
      // Initialize and update the area chart
      this.initializeChart();
      this.updateAreaChart(); // Update with area chart data
    } else {
      // Destroy the area chart if it exists
      if (this.chart) {
        this.chart.destroy();
        this.chart = null;
      }
      // Initialize and update the radar chart
      if (this.latestSensorValues) {
        // Determine the timestamps based on the selected duration
        const selectedDuration = parseInt(this.selectedDuration, 10);
        const timestamps = this.getLastTimestamps(this.latestSensorValues, selectedDuration);

        if (timestamps) {
          // Compute average values based on the filtered timestamps
          const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, timestamps, this.selectedWaterType);

          // Initialize and update radar chart with computed average values
          this.initializeRadialBar(averageValues);
          //this.updateRadarChart(averageValues);

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


  ShowChartType(): void {
    this.showAreaChart = !this.showAreaChart; // Toggle the chart type flag
    console.log('showAreaChart:', this.showAreaChart); // Log the current state of showAreaChart

    // Increment the chart key to force re-render
    this.chartKey++;

    if (this.showAreaChart) {
      // Destroy the radar chart if it exists
      if (this.chartRadar) {
        this.chartRadar.destroy();
        this.chartRadar = null;
      }
      // Initialize and update the area chart
      this.initializeChart();
      this.updateAreaChart(); // Update with area chart data
    } else {
      // Destroy the area chart if it exists
      if (this.chart) {
        this.chart.destroy();
        this.chart = null;
      }
      // Initialize and update the radar chart
      if (this.latestSensorValues) {
        // Determine the timestamps based on the selected duration
        const selectedDuration = parseInt(this.selectedDuration, 10);
        const timestamps = this.getLastTimestamps(this.latestSensorValues, selectedDuration);

        if (timestamps) {
          // Compute average values based on the filtered timestamps
          const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, timestamps, this.selectedWaterType);

          // Initialize and update radar chart with computed average values
          this.initializeRadialBar(averageValues);
          //this.updateRadarChart(averageValues);

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
  
    console.log('Updated lastFiveTimestamps:', this.lastFiveTimestamps);
  
    // Determine which chart to update based on the current chart type
    if (this.showAreaChart) {
      // Update area chart with new data
      this.updateAreaChart();
    } else {
      // Compute average values based on the latest sensor values and lastFiveTimestamps
      if (this.lastFiveTimestamps && this.lastFiveTimestamps.length > 0) {
        const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, this.lastFiveTimestamps, this.selectedWaterType);
        console.log('Computed averageValues for radar chart:', averageValues);
        
        // Destroy the existing radar chart if it exists
        if (this.chartRadar) {
          this.chartRadar.destroy();
          this.chartRadar = null;
        }
  
        // Initialize a new radar chart with the computed average values
        this.initializeRadialBar(averageValues);
      } else {
        console.warn('No timestamps available for the selected duration.');
      }
    }
  }
  

  

  onWaterTypeChange(): void {
    console.log('Selected Water Type:', this.selectedWaterType);
  
    // Determine which chart to update based on the current chart type
    if (this.showAreaChart) {
      // Update area chart with new data
      this.updateAreaChart();
    } else {
      // Compute average values based on the latest sensor values and lastFiveTimestamps
      if (this.latestSensorValues && this.lastFiveTimestamps) {
        const averageValues = this.computeAverageValuesAcrossTimestamps(this.latestSensorValues, this.lastFiveTimestamps, this.selectedWaterType);
        console.log('Computed averageValues for radar chart:', averageValues);
  
        // Destroy the existing radar chart if it exists
        if (this.chartRadar) {
          this.chartRadar.destroy();
          this.chartRadar = null;
        }
  
        // Initialize a new radar chart with the computed average values
        this.initializeRadialBar(averageValues);
      } else {
        console.warn('Not enough data to update the radar chart.');
      }
    }
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
        } = { Timestamp: timestamp };
  
        // Check if sensorValue exists and matches the selected water type
        if (sensorValue && sensorValue[timestamp]?.["Water Type"]!== 'FLOATING PROBE' && sensorValue[timestamp]?.["Water Type"] === this.selectedWaterType) {          
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

  
  updateRadarChart(averageValues: number[]) {
    if (this.chartRadar) {
      // Update the radar chart with the computed average values
      const series = averageValues.map((value, index) => ({
        name: ['Temperature', 'Potential Hydrogen', 'Total Dissolved Solids', 'Dissolved Oxygen'][index],
        data: [value]
      }));
  
      console.log('Updating radar chart with series:', series);
  
      this.chartRadar.updateSeries(series);
    } else {
      console.warn('Radar chart is not initialized.');
    }
  }
  
  
  
  
  
  
  
  

  private getData(path: string): Observable<any> {
    return this.db.object<any>(path).valueChanges();
  }

  
  private getLastTimestamps(sensorValues: SensorValues, n: number): string[] | null {
    // Filter out timestamps with "FLOATING PROBE" water type
    const filteredTimestamps = Object.keys(sensorValues || {}).filter(timestamp => sensorValues[timestamp]["Water Type"] !== 'FLOATING PROBE');
    const length = filteredTimestamps.length;
  
    console.log('Filtered timestamps:', filteredTimestamps);
    console.log('Requested number of timestamps:', n);
  
    if (length >= n) {
      const lastTimestamps = filteredTimestamps.slice(-n);
      console.log('Returning last N timestamps:', lastTimestamps);
      return lastTimestamps;
    } else if (length > 0) {
      // Return all available filtered timestamps if there are fewer than the requested number
      console.log('Returning all available filtered timestamps:', filteredTimestamps);
      return filteredTimestamps;
    } else {
      // No timestamps available
      console.warn('No timestamps available.');
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

  private initializeRadialBar(averageValues: number[]): void {
    if (!this.chartRadar) {
      // Create an ApexCharts instance for the chart
      this.chartRadar = new ApexCharts(document.getElementById('chart1'), this.getRadialBarOptions(averageValues));
      // Render the chart
      this.chartRadar.render();
    } else {
      // Update the chart if it already exists
      this.chartRadar.updateOptions(this.getRadialBarOptions(averageValues));
    }
  }

  private getChartOptions(): ApexCharts.ApexOptions {
    const seriesColors = [];
    const dataLabelColors = [];
    const yAxisColors = ['#FFFFFF']; // Default color for first y-axis
  
    // Configure colors and styles based on sensor visibility
    if (this.showTemperature) {
      seriesColors.push('#509BA8'); // Green color for Temperature series
      dataLabelColors.push('#509BA8'); // Label color for Temperature series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)
      
    }
    if (this.showPotentialHydrogen) {
      seriesColors.push('#F4BB00'); // Yellow color for Potential Hydrogen series
      dataLabelColors.push('#F4BB00'); // Label color for Potential Hydrogen series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)

    }
    if (this.showTDS) {
      seriesColors.push('#E22800'); // Red color for Total Dissolved Solids series
      dataLabelColors.push('#E22800'); // Label color for Total Dissolved Solids series
      yAxisColors.push('#FFFFFF'); // Color for second y-axis (Dissolved Oxygen)

    }
    if (this.showDO) {
      seriesColors.push('#65469E'); // Blue color for Dissolved Oxygen series
      dataLabelColors.push('#65469E'); // Label color for Dissolved Oxygen series
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
        type: 'line',
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


  private getRadialBarOptions(averageValues: number[]): ApexCharts.ApexOptions {
    const minValues = [-5, 1, 0, 0]; // Define minimum values for each parameter
    const maxValues = [40, 14, 10000, 20]; // Define maximum values for each parameter
  
    // Convert average values to percentages
    const percentageValues = averageValues.map((value, index) => {
      const min = minValues[index];
      const max = maxValues[index];
      return ((value - min) / (max - min)) * 100;
    });

    
    const units = ['°C', 'pH', 'ppm', 'mg/L']; // Units for each parameter
    return {
      series: percentageValues, // Use the percentage values for each parameter
      chart: {
        height: 450,
        type: 'radialBar',
        offsetY: 0,
        events: {
          dataPointMouseEnter: function(event, chartContext, config) {
            const index = config.dataPointIndex;
            const actualValue = averageValues[index];
            chartContext.updateOptions({
              plotOptions: {
                radialBar: {
                  dataLabels: {
                    total: {
                      label: config.w.globals.labels[index],
                      formatter: function() {
                        return actualValue.toFixed(2); // Return the actual value with two decimal places
                      }
                    }
                  }
                }
              }
            }, false, false);
          },
          dataPointMouseLeave: function(event, chartContext, config) {
            chartContext.updateOptions({
              plotOptions: {
                radialBar: {
                  dataLabels: {
                    total: {
                      formatter: function() {
                        const total = averageValues.reduce((acc, val) => acc + val, 0);
                        return total.toFixed(2); // Return the total value with two decimal places
                      }
                    }
                  }
                }
              }
            }, false, false);
          }
        }
      },
      plotOptions: {
        radialBar: {
          startAngle: 0,
          endAngle: 270,
          hollow: {
            background: 'transparent',
            image: undefined,
            imageOffsetX: 0,
            imageOffsetY: 0,
            position: 'front',
            dropShadow: {
              enabled: true,
              top: 3,
              left: 0,
              blur: 4, // Correct property for dropShadow
              opacity: 0.24
            }
          },
          track: {
            background: '#e0e0e0',
            strokeWidth: '70%',
            margin: 5, // margin is in pixels
            dropShadow: {
              enabled: true,
              top: -3,
              left: 0,
              blur: 4, // Correct property for dropShadow
              opacity: 0.15
            }
          },
          dataLabels: {
            show: true,
            name: {
              offsetY: -10,
              show: false,
              color: '#FFF67F',
              fontSize: '12px'
            },
            value: {
                formatter: function(val) {
                  // Default formatter for value
                  return val.toFixed(2); // Return the value with two decimal places
                },
              color: '#FFFFFF',
              fontSize: '50px',
              show: true,
              offsetY: 16
            },
            total: {
              show: true,
              formatter: function() {
                const total = averageValues.reduce((acc, val) => acc + val, 0);
                return total.toFixed(2) ; // Return the total value with two decimal places
              },
              color: '#FFFFFF',
              fontSize: '16px'
            }
          },
          barLabels: {
            enabled: true,
            useSeriesColors: true,
            margin: 20,
            fontSize: '16px',
            formatter: function(seriesName, opts) {
              return seriesName + ' ' + '(' + units[opts.seriesIndex] + ')';
            },
          },
        }
      },
      stroke: {
        lineCap: 'round'
      },
      colors: ['#509BA8', '#F5BC00', '#E62300', '#67409E'], // Custom colors for the legend labels
      labels: ['Temperature', 'Potential Hydrogen', 'Total Dissolved Solids', 'Dissolved Oxygen'],
      
      
      responsive: [{
        breakpoint: 480,
        options: {
          legend: {
            show: false
          }
        }
      }]
    };
  }
  
  
  
      
}




  