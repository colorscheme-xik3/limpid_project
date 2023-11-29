import { Component } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/compat/auth';
import { Router } from '@angular/router';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent {
  username: string = '';
  password: string = '';

  constructor(private auth: AngularFireAuth, private router: Router) {}

  login() {
    // Check if the form is valid before proceeding
    if (this.isValidForm()) {
      // Perform login with AngularFireAuth
      this.auth.signInWithEmailAndPassword(this.username, this.password)
        .then((userCredential) => {
          console.log('Logged in successfully!', userCredential.user);
          // Navigate to the dashboard after successful login
          this.router.navigate(['/dashboard']);
        })
        .catch((error) => {
          console.error('Login error:', error.message);
        });
    } else {
      console.error('Form is not valid.');
    }
  }

  // Function to check if the form is valid
  isValidForm(): boolean {
    // Add your custom validation logic here if needed
    // For now, just check if username and password are not empty
    return this.username.trim() !== '' && this.password.trim() !== '';
  }

}
