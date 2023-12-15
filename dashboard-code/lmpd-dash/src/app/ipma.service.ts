import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class IpmaService {
  private apiUrl = 'http://api.ipma.pt/open-data/forecast/oceanography/daily/hp-daily-sea-forecast-day0.json';
  private TempUrl = 'http://api.ipma.pt/open-data/forecast/meteorology/cities/daily/hp-daily-forecast-day0.json';

  constructor(private http: HttpClient) {}

  getSeaConditions(globalIdLocal: number): Observable<any> {
    const url = `${this.apiUrl}?globalIdLocal=${globalIdLocal}`;
    return this.http.get(url);
  }

  getTempConditions(globalIdLocal: number): Observable<any> {
    const url = `${this.TempUrl}?globalIdLocal=${globalIdLocal}`;
    return this.http.get(url);
  }

  
  
}
