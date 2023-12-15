// wrapper.component.ts
import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { Router } from '@angular/router';
import { IpmaService } from 'src/app/ipma.service';


@Component({
  selector: 'app-wrapper',
  templateUrl: './wrapper.component.html',
  styleUrls: ['./wrapper.component.css', './weather.css']
})
export class WrapperComponent implements OnInit {
  user: any; // Assuming you have a variable to store user information
  tempConditions: any;

  constructor(private auth: AngularFireAuth, private router: Router, private ipma: IpmaService) {}

  ngOnInit(): void {
    this.auth.user.subscribe((user) => {
      this.user = user;

      // Fetch sea conditions when the component initializes
      if (user) {
        this.fetchTempConditions();
      }
    });
  }

  fetchTempConditions(): void {
    // Replace '1111026' with your desired globalIdLocal
    this.ipma.getTempConditions(1151200).subscribe((data) => {
      // Filter the data based on globalIdLocal
      this.tempConditions = data.data.filter((condition: { globalIdLocal: number }) => condition.globalIdLocal === 1151200);
      // Log the filtered data to the console
      console.log('Filtered Temp Conditions:', this.tempConditions);
    });
  }

  getWeatherClass(tMax: number | undefined): string {

    if (tMax === undefined) {
      return ''; // or a default class if necessary
    } else if (tMax < 10) {
      return 'snowe';
    } else if (tMax > 30) {
      return 'hot';
    } else {
      return 'cloudy';
    }
  }

  logout(): void {
    // Your logout logic, for example, using AngularFireAuth
    this.auth.signOut().then(() => {
      // Redirect to the root path after logout
      this.router.navigate(['']);
    });
  }
}
