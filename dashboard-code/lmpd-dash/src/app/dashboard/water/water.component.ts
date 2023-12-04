import { Component, OnInit } from '@angular/core';
import ApexCharts from 'apexcharts';

@Component({
  selector: 'app-water',
  templateUrl: './water.component.html',
  styleUrls: ['./water.component.css']
})
export class WaterComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  
    // Sample data for the first chart
    const chartData1 = {
      series: [
        {
          name: "High - 2013",
          data: [28, 29, 33, 36, 32, 32, 33]
        },
        {
          name: "Low - 2013",
          data: [12, 11, 14, 18, 17, 13, 13]
        }
      ],
      chart: {
        height: 350,
        type: 'line',
        dropShadow: {
          enabled: true,
          color: '#000',
          top: 18,
          left: 7,
          blur: 10,
          opacity: 0.2
        },
        toolbar: {
          show: false
        }
      },
      colors: ['#77B6EA', '#545454'],
      dataLabels: {
        enabled: true,
      },
      stroke: {
        curve: 'smooth'
      },
      title: {
        text: 'Average High & Low Temperature',
        align: 'left'
      },
      grid: {
        borderColor: '#e7e7e7',
        row: {
          colors: ['#f3f3f3', 'transparent'],
          opacity: 0.5
        },
      },
      markers: {
        size: 1
      },
      xaxis: {
        categories: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul'],
        title: {
          text: 'Month'
        }
      },
      yaxis: {
        title: {
          text: 'Temperature'
        },
        min: 5,
        max: 40
      },
      legend: {
        position: 'top',
        horizontalAlign: 'right',
        floating: true,
        offsetY: -25,
        offsetX: -5
      },
      tooltip: {
        shared: true,
        intersect: false,
        theme: 'dark',
      },
    };

    // Create an ApexCharts instance for the first chart
    const chart1 = new ApexCharts(document.getElementById('chart1'), chartData1);

    // Render the first chart
    chart1.render();

    // Sample data for the second chart
    const chartData2 = {
      series: [{
        name: 'Sample Series 2',
        data: [20, 35, 50, 65, 80, 95, 110, 125, 140],
      }],
      chart: {
        type: 'bar',
        height: 350,
        // Apply the dark mode theme
        background: 'rgba(0, 0, 0, 0.5)', // Set background color
      },
      tooltip: {
        theme: 'dark',
      },
      xaxis: {
        categories: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep'],
      },
    };

    // Create an ApexCharts instance for the second chart
    const chart2 = new ApexCharts(document.getElementById('chart2'), chartData2);

    // Render the second chart
    chart2.render();
  }
}
