import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SurfComponent } from './surf.component';

describe('SurfComponent', () => {
  let component: SurfComponent;
  let fixture: ComponentFixture<SurfComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SurfComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SurfComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
