import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { IpmaService } from 'src/app/ipma.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css'],
})
export class HomeComponent implements OnInit {
  user: any; // Assuming you have a variable to store user information
  seaConditions: any;

  constructor(private auth: AngularFireAuth, private ipma: IpmaService) {}

  ngOnInit(): void {
    // Fetch user information
    this.auth.user.subscribe((user) => {
      this.user = user;

      // Fetch sea conditions when the component initializes
      if (user) {
        this.fetchSeaConditions();
      }
    });
  }

  // Function to fetch sea conditions
  fetchSeaConditions(): void {
    // Replace '1111026' with your desired globalIdLocal
    this.ipma.getSeaConditions(1111026).subscribe((data) => {
      // Filter the data based on globalIdLocal
      this.seaConditions = data.data.filter((condition: { globalIdLocal: number }) => condition.globalIdLocal === 1111026);
  
      // Log the filtered data to the console
      console.log('Filtered Sea Conditions:', this.seaConditions);
    });
  }

  getGreeting(): string {
    const currentTime = new Date();
    const hours = currentTime.getHours();
    const is24HourFormat = this.user?.timeFormat === '24h'; // Adjust the field name based on your user model
  
    let greeting = '';
  
    if (is24HourFormat) {
      if (hours < 6) {
        greeting = 'Good night';
      } else if (hours < 12) {
        greeting = 'Good morning';
      } else if (hours < 18) {
        greeting = 'Good afternoon';
      } else {
        greeting = 'Good evening';
      }
    } else {
      const amPm = hours < 12 ? 'AM' : 'PM';
      const formattedHours = hours % 12 || 12;
  
      if (hours >= 0 && hours < 6) {
        greeting = 'Good night';
      } else if (hours >= 6 && hours < 12) {
        greeting = 'Good morning';
      } else if (hours >= 12 && hours < 18) {
        greeting = 'Good afternoon';
      } else {
        greeting = 'Good evening';
      }
  
      return `${greeting}, ${this.user?.email || 'Guest'}`;
    }
  
    return `${greeting}, ${this.user?.email || 'Guest'}`;
  }
}
