// dashboard.component.ts
import { Component, OnInit, OnDestroy } from '@angular/core';
import { SharedService } from '../shared.service';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.css'],
})
export class DashboardComponent implements OnInit, OnDestroy {
  selectedButton: string = ''; // Initialize selectedButton
  private subscription: Subscription = new Subscription(); // Initialize subscription

  constructor(private sharedService: SharedService) {}

  ngOnInit() {
    this.subscription = this.sharedService.selectedButton$.subscribe((button) => {
      this.selectedButton = button;
    });
  }

  ngOnDestroy() {
    // Unsubscribe to avoid memory leaks
    this.subscription.unsubscribe();
  }
}
