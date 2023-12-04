import { Component, OnInit } from "@angular/core";
import {
  ApexAxisChartSeries,
  ApexNonAxisChartSeries,  // Add this import
  ApexChart,
  ApexTitleSubtitle,
  ApexDataLabels,
  ApexFill,
  ApexMarkers,
  ApexYAxis,
  ApexXAxis,
  ApexTooltip
} from "ng-apexcharts";
//import { waterTemperatureData } from "./water-temperature-data";

@Component({
  selector: "app-water",
  templateUrl: "./water.component.html",
  styleUrls: ["./water.component.css"]
})
export class WaterComponent implements OnInit {
  public chartOptions: {
    series: (ApexAxisChartSeries | ApexNonAxisChartSeries)[]; // Change this line
    chart: ApexChart;
    dataLabels: ApexDataLabels;
    markers: ApexMarkers;
    title: ApexTitleSubtitle;
    fill: ApexFill;
    yaxis: ApexYAxis;
    xaxis: ApexXAxis;
    tooltip: ApexTooltip;
  } = {
    series: [], // Initialize as an empty array
    chart: {
      type: "area",
      stacked: false,
      height: 350,
      zoom: {
        type: "x",
        enabled: true,
        autoScaleYaxis: true
      },
      toolbar: {
        autoSelected: "zoom"
      }
    },
    dataLabels: {
      enabled: false
    },
    markers: {
      size: 0
    },
    title: {
      text: "Water Temperature Over Time",
      align: "left"
    },
    fill: {
      type: "gradient",
      gradient: {
        shadeIntensity: 1,
        inverseColors: false,
        opacityFrom: 0.5,
        opacityTo: 0,
        stops: [0, 90, 100]
      }
    },
    yaxis: {
      labels: {
        formatter: (val: number) => val.toFixed(2),
      },
      title: {
        text: "Temperature (°C)"
      }
    },
    xaxis: {
      type: "datetime"
    },
    tooltip: {
      shared: false,
      y: {
        formatter: (val: number) => val.toFixed(2),
      }
    }
  };

  constructor() {}

  ngOnInit(): void {
    this.initChartData();
  }

  private initChartData(): void {
    // Example data for testing
    const exampleData = [
      10, 12, 14, 18, 20, 22, 24, 26, 28, 30, 25, 22, 20, 18, 16, 14, 12, 10, 8, 6,
      8, 10, 12, 14, 18, 20, 22, 24, 26, 28, 30, 25, 22, 20, 18, 16, 14, 12, 10, 8, 6,
      // ... add more values as needed
    ];
  
    let ts2 = 1484418600000;
    let dataPoints = [];
  
    for (let i = 0; i < exampleData.length; i++) {
      ts2 = ts2 + 86400000;
      dataPoints.push({ x: ts2, y: exampleData[i] });
    }
  
    this.chartOptions.series = [
      {
        name: "Water Temperature",
        data: dataPoints
      } as any // Use type assertion to 'any' for flexibility
    ];
  }
}
