import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { AngularFireDatabase } from '@angular/fire/compat/database';
import { Observable, BehaviorSubject } from 'rxjs';
import { switchMap, startWith, distinctUntilChanged } from 'rxjs/operators';
import ApexCharts from 'apexcharts';

@Component({
  selector: 'app-water',
  templateUrl: './water.component.html',
  styleUrls: ['./water.component.css']
})
export class WaterComponent implements OnInit {
  sensorValues$: Observable<{ [key: string]: { "Temperature": string, "Potential Hydrogen": string } }> | null = null;
  lastTimestamp: string[] | null = null;
  lastFiveTimestamps: string[] | null = null;
  private sensorValuesSubject = new BehaviorSubject<{ [key: string]: { Temperature: string, "Potential Hydrogen": string } } | null>(null);
  private chart: ApexCharts | null = null;
  showTemperature: boolean = true; // Variable to control temperature visibility
  showPotentialHydrogen: boolean = true; // Variable to control potential hydrogen visibility

  constructor(private db: AngularFireDatabase, private afAuth: AngularFireAuth) {}

  ngOnInit() {
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
      this.sensorValuesSubject.next(sensorValues || null);

      // Get the last timestamp when sensorValues are received
      this.lastTimestamp = this.getLastTimestamps(sensorValues, 1);
      this.lastFiveTimestamps = this.getLastTimestamps(sensorValues, 5);

      // Initialize and render ApexCharts when sensorValues are received
      this.initializeChart();

      // Update the chart when sensorValues are received
      this.updateChart();
    });
  }

  updateChart() {
  if (this.chart && this.lastFiveTimestamps && this.lastFiveTimestamps.length > 0 && this.sensorValuesSubject) {
    const filteredSensorValues = this.lastFiveTimestamps.map(timestamp => {
      const sensorValue = this.sensorValuesSubject.value;
      const entry: { Timestamp: string, Temperature?: number, "Potential Hydrogen"?: number } = { Timestamp: timestamp };

      // Assert that sensorValue is not null before accessing properties
      if (sensorValue) {
        if (this.showTemperature) {
          entry.Temperature = parseFloat(sensorValue[timestamp]?.Temperature);
        }
        if (this.showPotentialHydrogen) {
          entry["Potential Hydrogen"] = parseFloat(sensorValue[timestamp]?.['Potential Hydrogen']);
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

  private getChartOptions(): ApexCharts.ApexOptions {
    return {
      series: [],
      chart: {
        height: 450,
        type: 'area',
        toolbar: {
          show: false,
        },
      },
      colors: ['#008000', '#f5b74f'],
      dataLabels: {
        enabled: true,
        style: {
          colors: ['#008080', '#f5b74f'],
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
              color: '#008080',
            },
          },
          labels: {
            style: {
              colors: '#008080',
            },
          },
        },
        {
          opposite: true,
          title: {
            style: {
              color: '#f5b74f',
            },
          },
          labels: {
            style: {
              colors: '#f5b74f',
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
    };
  }
}
