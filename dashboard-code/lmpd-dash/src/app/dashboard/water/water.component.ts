// water.component.ts
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

    });
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
    // Use this.lastFiveTimestamps to filter the data for the last 5 timestamps when updating the chart
    if (this.lastFiveTimestamps && this.lastFiveTimestamps.length > 0 && this.sensorValuesSubject) {
      const filteredSensorValues = this.lastFiveTimestamps.map(timestamp => {
        const sensorValue = this.sensorValuesSubject.value;

        // Assert that sensorValue is not null before accessing properties
        if (sensorValue) {
          return {
            Timestamp: timestamp,
            ...sensorValue[timestamp] // Assuming the structure of your sensor values
          };
        }

        return null; // Handle the case where sensorValue is null
      }).filter(entry => entry !== null) as { Timestamp: string; Temperature: string; "Potential Hydrogen": string }[];

      // Assuming you have a chart variable declared in your class
      if (!this.chart) {
        // Create an ApexCharts instance for the chart
        this.chart = new ApexCharts(document.getElementById('chart1'), this.getChartOptions());
        // Render the chart
        this.chart.render();
      } else {
        // Update the chart data
        this.chart.updateSeries([
          {
            name: 'Temperature',
            data: filteredSensorValues.map(entry => parseFloat(entry.Temperature)),
          },
          {
            name: 'Potential Hydrogen',
            data: filteredSensorValues.map(entry => parseFloat(entry["Potential Hydrogen"])),
          },
        ]);
        // Update the x-axis labels
        this.chart.updateOptions({
          xaxis: {
            categories: filteredSensorValues.map(entry => entry.Timestamp),
          },
        });
      }
    } else {
      // Handle the case where there are no timestamps or not enough timestamps to update the chart
      console.warn('Not enough timestamps to update the chart.');
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
          colors: ['#008080', '#008080'],
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
            text: 'Temperature',
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
            text: 'Potential Hydrogen (pH)',
            style: {
              color: '#FFFFFF',
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
