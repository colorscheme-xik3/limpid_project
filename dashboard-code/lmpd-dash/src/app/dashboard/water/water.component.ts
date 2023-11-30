import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-water',
  templateUrl: './water.component.html',
  styleUrls: ['./water.component.css']
})
export class WaterComponent implements OnInit {

  constructor() { }

  // water.component.ts
ngOnInit() {
  console.log('WaterComponent initialized');
}


}
