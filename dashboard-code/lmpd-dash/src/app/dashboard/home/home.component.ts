import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
/// home.component.ts
export class HomeComponent implements OnInit {
  user: any; // Assuming you have a variable to store user information

  constructor(private auth: AngularFireAuth) {}

  ngOnInit(): void {
    // Fetch user information
    this.auth.user.subscribe((user) => {
      this.user = user;
    });
  }

  getGreeting(): string {
    const currentTime = new Date();
    const hours = currentTime.getHours();
    const is24HourFormat = this.user?.timeFormat === '24h'; // Adjust the field name based on your user model
  
    let greeting = '';
  
    if (is24HourFormat) {
      if (hours < 6) {
        greeting = 'Good Night';
      } else if (hours < 12) {
        greeting = 'Good Morning';
      } else if (hours < 18) {
        greeting = 'Good Afternoon';
      } else {
        greeting = 'Good Evening';
      }
    } else {
      const amPm = hours < 12 ? 'AM' : 'PM';
      const formattedHours = hours % 12 || 12;
  
      if (hours >= 0 && hours < 6) {
        greeting = 'Good Night';
      } else if (hours >= 6 && hours < 12) {
        greeting = 'Good Morning';
      } else if (hours >= 12 && hours < 18) {
        greeting = 'Good Afternoon';
      } else {
        greeting = 'Good Evening';
      }
  
      return `${greeting}, ${formattedHours}:${currentTime.getMinutes()} ${amPm} - ${this.user?.email || 'Guest'}`;
    }
  
    return `${greeting}, ${hours}:${currentTime.getMinutes()} - ${this.user?.email || 'Guest'}`;
  }
  
}





