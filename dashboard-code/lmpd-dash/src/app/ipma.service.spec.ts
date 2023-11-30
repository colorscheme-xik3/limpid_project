import { TestBed } from '@angular/core/testing';

import { IpmaService } from './ipma.service';

describe('IpmaService', () => {
  let service: IpmaService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(IpmaService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
