// wrapper.component.ts
import { Component, OnInit } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { Router } from '@angular/router';

@Component({
  selector: 'app-wrapper',
  templateUrl: './wrapper.component.html',
  styleUrls: ['./wrapper.component.css', './weather.css']
})
export class WrapperComponent implements OnInit {
  constructor(private auth: AngularFireAuth, private router: Router) {}

  ngOnInit(): void {}

  logout(): void {
    // Your logout logic, for example, using AngularFireAuth
    this.auth.signOut().then(() => {
      // Redirect to the root path after logout
      this.router.navigate(['']);
    });
  }
}
