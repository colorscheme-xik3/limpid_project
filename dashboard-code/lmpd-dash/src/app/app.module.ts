// app.module.ts
import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { RouterModule, Routes } from '@angular/router';
import { AngularFireModule } from '@angular/fire/compat';
import { AngularFireAuthModule } from '@angular/fire/compat/auth';

import { AppComponent } from './app.component';
import { LoginComponent } from './login/login.component';
import { environment } from '../environments/environment';
import { DashboardComponent } from './dashboard/dashboard.component';
import { WrapperComponent } from './wrapper/wrapper.component';
import { HomeComponent } from './dashboard/home/home.component';
import { WaterComponent } from './dashboard/water/water.component'; // Import the HomeComponent
import ApexCharts from 'apexcharts';
import { HttpClientModule } from '@angular/common/http';
import { SurfComponent } from './dashboard/surf/surf.component';



// Define your routes
const routes: Routes = [
  { path: '', component: LoginComponent },
  {
    path: 'dashboard',
    component: DashboardComponent,
    children: [
      { path: 'home', component: HomeComponent },
      { path: 'water', component: WaterComponent },

      // Add other child routes as needed
      { path: '', redirectTo: 'home', pathMatch: 'full' } // Default child route
    ],
  },
];

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    DashboardComponent,
    WrapperComponent,
    HomeComponent,
    WaterComponent,
    SurfComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AngularFireModule.initializeApp(environment.firebase),
    AngularFireAuthModule,
    RouterModule.forRoot(routes),
    HttpClientModule,
  ],
  
  bootstrap: [AppComponent],
})
export class AppModule {}
