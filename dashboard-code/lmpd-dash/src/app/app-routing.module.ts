// app-routing.module.ts
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { LoginComponent } from './login/login.component';
import { DashboardComponent } from './dashboard/dashboard.component';
import { HomeComponent } from './dashboard/home/home.component';
import { WaterComponent } from './dashboard/water/water.component';

const routes: Routes = [
  { path: 'login', component: LoginComponent },
  {
    path: 'dashboard',
    component: DashboardComponent,
    children: [
      { path: 'home', component: HomeComponent },
      { path: 'water', component: WaterComponent },
      { path: 'about', component: WaterComponent },

      // Add other child routes as needed
      { path: '', redirectTo: 'home', pathMatch: 'full' } // Default child route
    ],
  },
  { path: '**', redirectTo: 'login' }, // Redirect unmatched paths to the login page
];


@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule {}
