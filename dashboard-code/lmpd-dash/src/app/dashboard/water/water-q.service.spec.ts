import { TestBed } from '@angular/core/testing';

import { WaterQService } from './water-q.service';

describe('WaterQService', () => {
  let service: WaterQService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(WaterQService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
