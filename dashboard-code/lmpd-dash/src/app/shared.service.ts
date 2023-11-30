// shared.service.ts
import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class SharedService {
  private selectedButtonSource = new BehaviorSubject<string>('');
  selectedButton$ = this.selectedButtonSource.asObservable();

  setSelectedButton(button: string) {
    this.selectedButtonSource.next(button);
  }
}
