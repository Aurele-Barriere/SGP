/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return
//      immediately, and an interrupt happens later on).
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. In the ACIA Interrupt mode,
  initialize the reception index and semaphores and allow
  reception and emission interrupts.
  In the ACIA Busy Waiting mode, simply inittialize the ACIA
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------

DriverACIA::DriverACIA()
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
#endif
#ifdef ETUDIANTS_TP
  ind_send = 0;
  ind_rec  = 0;
  send_sema    = new Semaphore((char*) "ACIAdriver_send_semaphore",1);
  receive_sema = new Semaphore((char*) "ACIAdriver_recv_semaphore",0);
  if (g_cfg->ACIA == ACIA_BUSY_WAITING) {
    g_machine->acia->SetWorkingMode(BUSY_WAITING);
  } else if (g_cfg->ACIA == ACIA_INTERRUPT) {
    g_machine->acia->SetWorkingMode(REC_INTERRUPT);
  }
#endif

}

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------

int DriverACIA::TtySend(char* buff)
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
#endif
#ifdef ETUDIANTS_TP
  if (g_cfg->ACIA == ACIA_BUSY_WAITING) {
    int sent = 0;
    bool over = false;
    char c;

    while (!over) {
      while (g_machine->acia->GetOutputStateReg() == FULL) {
	// busy waiting
      }
      // sending a character
      c = buff[sent];
      g_machine->acia->PutChar(c);
      sent += 1;
      if (c == '\0') {
	over = true;
      }
    }
    return sent;
  } else { // with interrupts

    send_sema->P();
    int i = 0;
    for (i=0; i<BUFFER_SIZE; i++) {
      send_buffer[i] = buff[i];
      if (buff[i] == '\0') {
	break;
      }
    }

    // enabling interrupts
    int previous = g_machine->acia->GetWorkingMode();
    g_machine->acia->SetWorkingMode(previous | SEND_INTERRUPT);

    // sending first char
    char c = send_buffer[0];
    ind_send++;
    g_machine->acia->PutChar(c);


    return (i+1);
  }
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------

int DriverACIA::TtyReceive(char* buff,int lg)
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
#endif
#ifdef ETUDIANTS_TP

  if (g_cfg->ACIA == ACIA_BUSY_WAITING) {
    int received = 0;
    bool over = false;
    char c;
    while (!over) {
      while (g_machine->acia->GetInputStateReg() == EMPTY) {
	// busy waiting
      }
      // receiving a character
      c = g_machine->acia->GetChar();
      buff[received] = c;

      received += 1;

      if (received == lg) {
	over = true;
      }
    }
    return received;
  } else { // with interrupts

    int i = 0;


    // semaphore waiting
    receive_sema->P();


    int maxind = min(lg, ind_rec);
    for (i=0; i<maxind; i++) {
      buff[i] = receive_buffer[i];
    }
    ind_rec = 0;

    //enabling interrupts
    int previous = g_machine->acia->GetWorkingMode();
    g_machine->acia->SetWorkingMode(previous | REC_INTERRUPT);

    return i;
  }
#endif
}


//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only.
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptSend()
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
#endif
#ifdef ETUDIANTS_TP
  char c;
  c = send_buffer[ind_send];
  g_machine->acia->PutChar(c);

  ind_send++;

  if (c == '\0' || ind_send == BUFFER_SIZE) {
    ind_send = 0;
    send_sema->V();
    int previous = g_machine->acia->GetWorkingMode();
    g_machine->acia->SetWorkingMode(previous & ~SEND_INTERRUPT);
    printf("done\n");
  }

#endif
}

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA.
  Releases the receive_sema semaphore and disables reception
  interrupts when the last character of the message is received
  (character '\0').
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptReceive()
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
#endif
#ifdef ETUDIANTS_TP
  char c;
  c = g_machine->acia->GetChar();
  receive_buffer[ind_rec] = c;
  ind_rec++;
  if (c == '\0' || ind_rec == BUFFER_SIZE) {
    receive_sema->V();

    // disabling REC interrupts
    int previous = g_machine->acia->GetWorkingMode();
    g_machine->acia->SetWorkingMode(previous & ~REC_INTERRUPT);
  }
#endif
}
